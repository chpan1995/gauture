#include "HttpClient.h"
#include <iostream>

HttpClient::HttpClient(boost::asio::io_context &ioc)
    : m_ioc(ioc), m_resolver(boost::asio::make_strand(ioc)),
      m_stream(boost::asio::make_strand(ioc)) {}

void HttpClient::addRequest(HttpRequest request) {
  std::unique_lock<std::mutex> lk(m_mtx);
  m_pendingRequests.push(request);
  lk.unlock();
  if (!m_isProcessing) {
    processNextRequest();
  }
}

void HttpClient::processNextRequest() {
  if (m_pendingRequests.empty()) {
    m_isProcessing = false;
    return;
  }
  m_isProcessing = true;
  auto &request = m_pendingRequests.front();

  if (request.method == HttpRequest::HttpMethod::GET) {
    auto &req = m_req.emplace<
        boost::beast::http::request<boost::beast::http::empty_body>>();
    req.version(request.version);
    req.method(boost::beast::http::verb::get);
    req.target(request.target);
    req.set(boost::beast::http::field::host, request.host);
    req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  } else if (request.method == HttpRequest::HttpMethod::POST) {
    auto &req = m_req.emplace<
        boost::beast::http::request<boost::beast::http::string_body>>();
    req.version(request.version);
    req.method(boost::beast::http::verb::post);
    req.target(request.target);
    req.set(boost::beast::http::field::host, request.host);
    req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(boost::beast::http::field::content_type, "application/json");
    // req.set("X-Custom-Header", "MyValue");            // 设置自定义请求头
    req.body() = request.body;
    req.prepare_payload(); // 必须调用这个来设置content-length
  }

  std::visit(
      [&](auto &req) {
        for (auto &[key, value] : request.headers) {
          req.set(key, value);
        }
      },
      m_req);

  // 开始异步解析域名
#ifdef BCOROUTINE
  boost::asio::spawn(
      m_stream.get_executor(),  
      [&, self = shared_from_this()](boost::asio::yield_context yield) {
        try {
          const auto &results =
              self->m_resolver.async_resolve(request.host, request.port, yield);
          self->m_stream.expires_after(std::chrono::seconds(30));
          self->m_stream.async_connect(results, yield);
          std::visit([&](auto &req) {
                boost::beast::http::async_write(self->m_stream, req, yield);
              },self->m_req);
          self->m_buffer.consume(self->m_buffer.size());
          self->m_res = {};
          boost::beast::http::async_read(self->m_stream, self->m_buffer,
                                         self->m_res, yield);
          // m_res.result(); 回复状态码
          // 处理响应
          auto &current_request = self->m_pendingRequests.front();
          auto &body = self->m_res.body();
          current_request.callback(body.c_str(), body.length());

          // 清理当前连接
          boost::beast::error_code shutdown_ec;
          m_stream.socket().shutdown(
              boost::asio::ip::tcp::socket::shutdown_both, shutdown_ec);
          // 移除已完成的请求
          self->m_pendingRequests.pop();
          // 处理下一个请求
          processNextRequest();

        } catch (const std::exception &e) {
          handleError(std::string("operate error:") + e.what());
        }
      });
#else
  m_resolver.async_resolve(request.host, request.port,
                           boost::beast::bind_front_handler(
                               &HttpClient::on_resolve, shared_from_this()));
#endif
}

void HttpClient::on_resolve(
    boost::beast::error_code ec,
    boost::asio::ip::tcp::resolver::results_type results) {
  if (ec) {
    handleError("Resolve failed: " + ec.message());
    return;
  }

  m_stream.expires_after(std::chrono::seconds(30));
  m_stream.async_connect(
      results, boost::beast::bind_front_handler(&HttpClient::on_connect,
                                                shared_from_this()));
}

void HttpClient::on_connect(
    boost::beast::error_code ec,
    boost::asio::ip::tcp::resolver::results_type::endpoint_type) {
  if (ec) {
    handleError("Connect failed: " + ec.message());
    return;
  }
  m_stream.expires_after(std::chrono::seconds(30));

  std::visit(
      [&](auto &req) {
        boost::beast::http::async_write(
            m_stream, req,
            boost::beast::bind_front_handler(&HttpClient::on_write,
                                             shared_from_this()));
      },
      m_req);
}

void HttpClient::on_write(boost::beast::error_code ec,
                          std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    handleError("Write failed: " + ec.message());
    return;
  }

  m_buffer.consume(m_buffer.size());
  m_res = {};

  boost::beast::http::async_read(m_stream, m_buffer, m_res,
                                 boost::beast::bind_front_handler(
                                     &HttpClient::on_read, shared_from_this()));
}

void HttpClient::on_read(boost::beast::error_code ec,
                         std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    handleError("Read failed: " + ec.message());
    return;
  }
  // m_res.result(); 回复状态码
  // 处理响应
  auto &current_request = m_pendingRequests.front();
  auto &body = m_res.body();
  current_request.callback(body.c_str(), body.length());

  // 清理当前连接
  boost::beast::error_code shutdown_ec;
  m_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                             shutdown_ec);
  // 移除已完成的请求
  m_pendingRequests.pop();
  // 处理下一个请求
  processNextRequest();
}

void HttpClient::handleError(const std::string &error_msg) {
  auto &current_request = m_pendingRequests.front();
  auto msg =
      boost::json::serialize(boost::json::object({{"eMessage", error_msg}}));
  current_request.callback(msg.c_str(), msg.length());
  m_pendingRequests.pop();
  processNextRequest();
}
