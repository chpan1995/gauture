#include "HttpClient.h"

#include <iostream>
// http://192.168.1.108:8083/tasks/info
int main(int argc, char **argv)
{
    boost::asio::io_context ioc;
    auto client = std::make_shared<HttpClient>(ioc);

    // GET请求示例
    client->addRequest(HttpRequest(
        "192.168.1.1081",
        "8083",
        "/tasks/info",
        [](const char *response, std::size_t lenth)
        {
            std::cout << "GET Response: " << response << std::endl;
        }));
    client->addRequest(HttpRequest(
        "192.168.1.108",
        "8083",
        "/tasks/info",
        [](const char *response, std::size_t lenth)
        {
            std::cout << "GET Response: " << response << std::endl;
        }));
    // POST请求示例
    std::string json_body = R"({
        "name": "John Doe",
        "email": "john@example.com"
    })";

    HttpRequest post_req(
        "api.example.com",
        "80",
        "/users",
        json_body,
        [](const char* response,std::size_t lenth) {
            std::cout << "POST Response: " << response << std::endl;
        }
    );

    // 添加自定义头部
    post_req.headers["Authorization"] = "Bearer token123";
    client->addRequest(std::move(post_req));

    ioc.run();
}