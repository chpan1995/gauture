#include "LabelImgData.h"

LabelImgData::LabelImgData(QObject *parent)
    : QObject(parent)
{
    m_HttpClient = std::make_shared<HttpClient>(m_ioc);
    m_thd = std::thread([this] {
        boost::asio::io_context::work work(m_ioc);
        m_ioc.run();
    });
}

LabelImgData::~LabelImgData()
{
    m_ioc.stop();
    if (m_thd.joinable())
        m_thd.join();
}

void LabelImgData::requestImgInfo()
{
    m_HttpClient->addRequest(
        HttpRequest("192.168.1.108",
                    "8083",
                    "/tasks/info",
                    [this](const char *response, std::size_t lenth) {
                        boost::system::error_code ec;
                        auto v = praseRespose(response, lenth);
                        if (v.has_value()) {
                            auto objArry = v.value().find_pointer("/message", ec);
                            if (!ec && objArry->is_array()) {
                                for (auto &obj : objArry->as_array()) {
                                    auto count = obj.find_pointer("/count", ec);
                                    if (!ec && count->if_int64()) {
                                        qDebug() << count->get_int64();
                                    }

                                    auto create_time = obj.find_pointer("/create_time", ec);
                                    if (!ec && create_time->is_string()) {
                                        qDebug() << create_time->get_string().c_str();
                                    }

                                    auto name = obj.find_pointer("/name", ec);
                                    if (!ec && name->is_string()) {
                                        qDebug() << name->get_string().c_str();
                                    }
                                }
                            }
                        }
                    }));
}

void LabelImgData::requestImgName(QString name)
{
    m_HttpClient->addRequest(HttpRequest("192.168.1.108",
                                         "8083",
                                         QString(QString("/tasks/pull/") + name).toStdString(),
                                         [this](const char *response, std::size_t lenth) {
                                             auto v = praseRespose(response, lenth);
                                             if (v.has_value()) {
                                                 boost::system::error_code ec;
                                                 auto objArry = v.value().find_pointer("/message", ec);
                                                 if (!ec && objArry->is_array()) {
                                                     for (auto &it : objArry->as_array()) {
                                                         if(it.is_string()) {
                                                             m_imgNames.append(it.get_string().c_str());
                                                         }
                                                     }
                                                 }
                                             }
                                         }));
}

std::optional<boost::json::value> LabelImgData::praseRespose(const char *response, std::size_t lenth)
{
    boost::json::value v;
    boost::json::stream_parser p;
    boost::system::error_code ec;
    p.write(response, lenth, ec);
    if (ec) {
        qDebug() << "-> parse json failed";
        return {};
    } else {
        v = p.release();
        auto err = v.find_pointer("/eMessage", ec);
        if (!ec) {
            // 请求错误
            qDebug() << err->get_string().c_str();
            return {};
        }
    }
    return v;
}
