#ifndef WEBSCOKETCLIENT_H
#define WEBSCOKETCLIENT_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>
#include <string>
#include <thread>

class WebscoketSession : public std::enable_shared_from_this<WebscoketSession>
{
public:
    explicit WebscoketSession(boost::asio::io_context &ioc,
                              std::string host,
                              std::string port,
                              std::string pram);
    void run();
    void send(std::string);

private:
    void on_resolve(boost::beast::error_code ec,
                    boost::asio::ip::tcp::resolver::results_type results);
    void on_connect(boost::beast::error_code ec,
                    boost::asio::ip::tcp::resolver::results_type::endpoint_type ep);
    void on_handshake(boost::beast::error_code ec);
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

    void tmEntrty(const boost::system::error_code &ec);
    void keepAliveEntrty(const boost::system::error_code &ec);

private:
    boost::asio::ip::tcp::resolver m_resolver;
    std::shared_ptr<boost::beast::websocket::stream<boost::beast::tcp_stream>> m_ws;
    boost::beast::flat_buffer m_buffer;
    boost::asio::steady_timer m_timer;
    boost::asio::steady_timer m_keeplAlive;
    std::string m_host;
    std::string m_port;
    std::string m_pram;
    bool m_run{false};
    boost::asio::io_context& m_ioc;
};

class WebscoketClient
{
public:
    explicit WebscoketClient(std::string username);
    ~WebscoketClient();
private:
    boost::asio::io_context m_ioc;
    std::thread m_thd;
    std::shared_ptr<WebscoketSession> m_webscoketSession;
};

#endif // WEBSCOKETCLIENT_H
