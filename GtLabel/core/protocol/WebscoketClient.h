#ifndef WEBSCOKETCLIENT_H
#define WEBSCOKETCLIENT_H

#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <boost/function.hpp>
#include <boost/bind/bind.hpp>
#include <string>

class WebscoketClient : public std::enable_shared_from_this<WebscoketClient>
{
public:
    explicit WebscoketClient(boost::asio::io_context& ioc,std::string host,std::string port,std::string pram);
    void run();
    void send(std::string);
private:
    void on_resolve(boost::beast::error_code ec,
        boost::asio::ip::tcp::resolver::results_type results);
    void on_connect(boost::beast::error_code ec,
        boost::asio::ip::tcp::resolver::results_type::endpoint_type ep);
    void on_handshake(boost::beast::error_code ec);
    void do_read();
    void on_read(boost::beast::error_code ec,std::size_t bytes_transferred);
    
    void tmEntrty(const boost::system::error_code &ec);
    void keepAliveEntrty(const boost::system::error_code &ec);
    void on_keepAlive(boost::beast::error_code ec,std::size_t bytes_transferred);
private:
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::websocket::stream<boost::beast::tcp_stream> m_ws;
    boost::beast::flat_buffer m_buffer;
    boost::asio::steady_timer m_timer;
    boost::asio::steady_timer m_keeplAlive;
    std::string m_host;
    std::string m_port;
    std::string m_pram;
    bool m_run {false};
};

#endif // WEBSCOKETCLIENT_H
