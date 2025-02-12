#include "WebscoketClient.h"
#include "log.h"

#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

WebscoketSession::WebscoketSession(boost::asio::io_context &ioc,
                                   std::string host,
                                   std::string port,
                                   std::string pram,
                                   WebscoketClient* parent)
    : m_resolver(boost::asio::make_strand(ioc)),m_ioc(ioc)
    , m_ws(std::make_shared<boost::beast::websocket::stream<boost::beast::tcp_stream>>(boost::asio::make_strand(ioc)))
    , m_host(host)
    , m_port(port)
    , m_pram(pram)
    , m_parent(parent)
    , m_timer(ioc, boost::asio::chrono::seconds(3))
    , m_keeplAlive(ioc, boost::asio::chrono::seconds(6))
{
    m_keeplAlive.async_wait(
        boost::bind(&WebscoketSession::keepAliveEntrty, this, boost::placeholders::_1));
}

void WebscoketSession::run()
{
    m_buffer.consume(m_buffer.size()); // 清空缓冲区
    // Look up the domain name
    m_resolver.async_resolve(m_host,
                             m_port,
                             boost::beast::bind_front_handler(&WebscoketSession::on_resolve,
                                                              shared_from_this()));
}

void WebscoketSession::on_resolve(boost::beast::error_code ec,
                                  boost::asio::ip::tcp::resolver::results_type results)
{
    if (ec) {
        logging::log_error(RL, "resolve:" + ec.message());
        return;
    }

    // Set the timeout for the operation
    boost::beast::get_lowest_layer(*m_ws).expires_after(std::chrono::seconds(6));

    // Make the connection on the IP address we get from a lookup
    boost::beast::get_lowest_layer(*m_ws)
        .async_connect(results,
                       boost::beast::bind_front_handler(&WebscoketSession::on_connect,
                                                        shared_from_this()));
}

void WebscoketSession::on_connect(boost::beast::error_code ec,
                                  boost::asio::ip::tcp::resolver::results_type::endpoint_type ep)
{
    if (ec) {
        logging::log_error(RL, "connect:" + ec.message());
        ;
        return;
    }
    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    boost::beast::get_lowest_layer(*m_ws).expires_never();

    // Set suggested timeout settings for the websocket
    m_ws->set_option(
        boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_ws->set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::request_type &req) {
            req.set(boost::beast::http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
        }));

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    std::string url = m_host + ':' + std::to_string(ep.port());

    // Perform the websocket handshake
    m_ws->async_handshake(url,
                         "/" + m_pram,
                         boost::beast::bind_front_handler(&WebscoketSession::on_handshake,
                                                          shared_from_this()));
}

void WebscoketSession::on_handshake(boost::beast::error_code ec)
{
    if (ec) {
        logging::log_error(RL, "handshake:" + ec.message());
        return;
    }
    m_run = true;
    m_timer.async_wait(boost::bind(&WebscoketSession::tmEntrty, this, boost::placeholders::_1));
    do_read();
}

void WebscoketSession::do_read()
{
    m_ws->async_read(m_buffer,
                    boost::beast::bind_front_handler(&WebscoketSession::on_read,
                                                     shared_from_this()));
}

void WebscoketSession::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    if (ec) {
        logging::log_error(RL, "read:" + ec.message());
        return;
    }
    if (m_ws->got_text()) {
        try {
            auto v = boost::json::parse(boost::beast::buffers_to_string(m_buffer.data()));
            if (v.is_object()) {
                boost::system::error_code ec;
                auto cmd = v.find_pointer("/commond", ec);
                if (!ec) {
                    if (cmd->get_string() == "keepAlive") {
                        m_run = true;
                    } else {
                        if(m_parent) {
                            for(auto it:m_parent->m_obs){
                                it->praseData(v);
                            }
                        }
                        // exit(0);
                        // logging::log_info(RL, boost::beast::buffers_to_string(m_buffer.data()));
                    }

                }
            }
        } catch (const std::exception &e) {
        }
    }

    m_buffer.consume(m_buffer.size());
    do_read();
}

void WebscoketSession::send(std::string v)
{
    if (m_ws->is_open()) {
        boost::system::error_code ec;
        m_ws->write(boost::asio::buffer(v), ec);
        if (ec) {
            logging::log_error(RL, "write:" + ec.message());
        }
        /* 如果 client->send 被多次调用（即使添加到了队列中），但上一条消息的写入尚未完成，async_write 会违反内部状态机逻辑，导致断言失败。
      关键点：WebSocket 的 async_write 是单线程的，但它要求不能有重叠的写入操作（即新一次写入不能在上一次写入完成前开始）。*/
        // boost::asio::post(m_ws.get_executor(), [self = shared_from_this(), v] {
        //   self->m_ws.text(true);
        //   self->m_ws.async_write(boost::asio::buffer(v),
        //                          [](boost::beast::error_code ec,std::size_t bytes_transferred) {
        //                             boost::ignore_unused(bytes_transferred);
        //                            if (ec) {
        //                              std::cout << "write failed:" + ec.message();
        //                            }
        //                          });
        // });
    }
}

void WebscoketSession::tmEntrty(const boost::system::error_code &ec)
{
    // 在回调函数中检查 boost::system::error_code
    // 如果定时器已被取消（ec == boost::asio::error::operation_aborted）
    if (ec == boost::asio::error::operation_aborted) {
        return; // 定时器被取消，退出回调
    }
    send(boost::json::serialize(boost::json::object{{"commond", "keepAlive"}}));
    m_timer.expires_after(boost::asio::chrono::seconds(3));
    m_timer.async_wait(boost::bind(&WebscoketSession::tmEntrty, this, boost::placeholders::_1));
}

void WebscoketSession::keepAliveEntrty(const boost::system::error_code &ec)
{
    if (ec == boost::asio::error::operation_aborted) {
        return; // 定时器被取消，退出回调
    }
    if (!m_run) {
        if(m_parent) {
            for(auto it:m_parent->m_obs){
                boost::json::object obj({{"commond","ServerDown"}});
                auto v=boost::json::value_from(obj);
                it->praseData(v);
            }
        }
        logging::log_info(RL, "reconnect");
        boost::system::error_code ec;
        // m_ws.close({""}, ec); // 会阻塞，异步多次会有问题
        // 强制关闭底层 TCP 连接
        auto &socket = boost::beast::get_lowest_layer(*m_ws).socket();
        // 尝试关闭 TCP 的接收和发送方向
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        socket.close();
        m_ws = std::make_shared<boost::beast::websocket::stream<boost::beast::tcp_stream>>(boost::asio::make_strand(m_ioc));
        run();
    }else {
        if(m_parent) {
            for(auto it:m_parent->m_obs){
                boost::json::object obj({{"commond","ServerUP"}});
                auto v=boost::json::value_from(obj);
                it->praseData(v);
            }
        }
    }
    m_keeplAlive.expires_after(boost::asio::chrono::seconds(6));
    m_keeplAlive.async_wait(
        boost::bind(&WebscoketSession::keepAliveEntrty, this, boost::placeholders::_1));
    m_run = false;
}

WebscoketClient::WebscoketClient(std::string username)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();

    // 获取微秒
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    m_webscoketSession = std::make_shared<WebscoketSession>(m_ioc,
                                                            "192.168.1.158",
                                                            "8081",
                                                            "?username=" + username+"_"+std::to_string(microseconds),this);
    m_webscoketSession->run();
    m_thd = std::thread([this] {
        auto work(boost::asio::make_work_guard(m_ioc));
        m_ioc.run();
    });
}

WebscoketClient::~WebscoketClient()
{
    m_ioc.stop();
    if (m_thd.joinable())
        m_thd.join();
}

void WebscoketClient::attach(std::shared_ptr<ObsWebContent> obs) {
    for (const auto &it : m_obs) if (it == obs) return;
    m_obs.push_back(obs);
}

void WebscoketClient::detach(std::shared_ptr<ObsWebContent> obs) {
    m_obs.remove(obs);
}
