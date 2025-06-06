#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/function.hpp>
#include <boost/json.hpp>
#include <queue>
#include <memory>
#include <functional>
#include <map>

// 定义请求结构体
struct HttpRequest {

    enum class HttpMethod : std::uint8_t {
        GET,
        POST
    };

    std::string host;
    std::string port;
    std::string target;
    HttpMethod method; 
    std::string body;
    int version;
    std::map<std::string, std::string> headers;  // 自定义头部
    boost::function2<void,const char*,std::size_t> callback; // 回调函数处理响应

    // GET请求构造函数
    HttpRequest(
        const std::string& h, 
        const std::string& p, 
        const std::string& t,
        boost::function2<void,const char*,std::size_t> cb,
        int v = 11)
        : host(h), port(p), target(t), callback(cb), version(v), method(HttpMethod::GET) {}

    // POST请求构造函数
    HttpRequest(
        const std::string& h, 
        const std::string& p, 
        const std::string& t,
        const std::string& b,
        boost::function2<void,const char*,std::size_t> cb,
        int v = 11)
        : host(h), port(p), target(t), body(b), callback(cb), version(v), method(HttpMethod::POST) {}
};

class HttpClient : public std::enable_shared_from_this<HttpClient> {
 public:

    explicit HttpClient(boost::asio::io_context& ioc);

    void addRequest(HttpRequest request);

 private:
    void processNextRequest();
    void on_resolve(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results);
    void on_connect(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type::endpoint_type);
    void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

    void handleError(const std::string& error_msg);
 private:
    boost::asio::io_context& m_ioc;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::tcp_stream m_stream;
    boost::beast::flat_buffer m_buffer;
    std::queue<HttpRequest> m_pendingRequests;
    std::mutex m_mtx;
    bool m_isProcessing {false};
    std::variant<
        boost::beast::http::request<boost::beast::http::empty_body>,
        boost::beast::http::request<boost::beast::http::string_body>
    > m_req;
    boost::beast::http::response<boost::beast::http::string_body> m_res;
};

#endif