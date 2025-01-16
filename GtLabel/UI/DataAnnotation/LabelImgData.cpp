#include "LabelImgData.h"
#include <boost/asio.hpp>

LabelImgData::LabelImgData(QObject *parent)
    : QObject(parent),m_taskInfoModel(new TaskInfoModel(this))
    , m_labelTags(new LabelTags(this))
{
    m_HttpClient = std::make_shared<HttpClient>(m_ioc);
    m_thd = std::thread([this] {
        auto work(boost::asio::make_work_guard(m_ioc));
        m_ioc.run();
    });
    qRegisterMetaType<LabelImgNamespace::RequestMethod>("RequestMethod");
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
                            emit request(true,LabelImgNamespace::RequestMethod::TasksInfo);
                            auto objArry = v.value().find_pointer("/message", ec);
                            QList<std::tuple<QString,QString,
                                             unsigned int,unsigned int,unsigned int>> das;
                            if (!ec && objArry->is_array()) {
                                QString taskName,taskNameCreateTime;
                                unsigned int taskImgCount;
                                std::tuple<QString,QString,
                                           unsigned int,unsigned int,unsigned int> tp;
                                for (auto &obj : objArry->as_array()) {
                                    auto count = obj.find_pointer("/count", ec);
                                    if (!ec && count->if_int64()) {
                                        taskImgCount = count->get_int64();
                                    }

                                    auto create_time = obj.find_pointer("/create_time", ec);
                                    if (!ec && create_time->is_string()) {
                                        taskNameCreateTime = create_time->get_string().c_str();
                                    }

                                    auto name = obj.find_pointer("/name", ec);
                                    if (!ec && name->is_string()) {
                                        taskName = name->get_string().c_str();
                                    }
                                    tp={taskName,taskNameCreateTime,taskImgCount,0,0};
                                    das.append(tp);
                                }

                                m_taskInfoModel->setDatas(das);
                                m_model=QVariant::fromValue(m_taskInfoModel);
                                emit taskInfoModelChanged();
                            }
                        }else {
                            emit request(false,LabelImgNamespace::RequestMethod::TasksInfo);
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
                                                 emit request(true,LabelImgNamespace::RequestMethod::TasksPull);
                                                 m_imgNames.clear();
                                                 m_labelTagsModels.clear();
                                                 boost::system::error_code ec;
                                                 auto objArry = v.value().find_pointer("/message", ec);
                                                 if (!ec && objArry->is_array()) {
                                                     for (auto &it : objArry->as_array()) {
                                                         if(it.is_string()) {
                                                             m_imgNames.append(it.get_string().c_str());
                                                         }
                                                     }
                                                 }
                                                 for(auto& it:m_imgNames) {
                                                     m_labelTagsModels.insert(it,QList<LabelTagsItem>());
                                                     m_currentTrait.insert(it,1);
                                                 }
                                                 if(m_imgNames.size()>0) {
                                                     m_imgName=m_imgNames[0];
                                                     emit imgNameChanged();
                                                     m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
                                                 }
                                             }else {
                                                 emit request(false,LabelImgNamespace::RequestMethod::TasksPull);
                                             }
                                         }));
}

void LabelImgData::gotoImgs(LabelImgNamespace::PageGo v) {
    switch (v) {
    case LabelImgNamespace::PageGo::Next:
    {
        int index=++m_currentPage;
        if(index > m_imgNames.size()) {
            m_currentPage--;
        }else {
            // 清除未点击标注信息
            if(m_isTaging) {
                m_labelTags->removeRows();
            }
            m_imgName=m_imgNames[index];
            emit imgNameChanged();
            m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
            m_isTaging=false;
        }
    }
        break;
    case LabelImgNamespace::PageGo::Front:
    {
        int index=--m_currentPage;
        if(index<0) {
            m_currentPage++;
        }else {
            if(m_isTaging) {
                m_labelTags->removeRows();
            }
            m_imgName=m_imgNames[index];
            emit imgNameChanged();
            m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
            m_isTaging=false;
        }
    }
        break;
    default:
        break;
    }


}

void LabelImgData::setTagStatus(bool f) {
    m_isTaging = f;
}

void LabelImgData::reset() {
    if(m_labelTagsModels.contains(m_imgName)) {
        m_currentTrait[m_imgName]=1;
        m_labelTagsModels[m_imgName].clear();
        m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
    }
}

bool LabelImgData::lab() {
    bool tmp = m_isTaging;
    if(m_isTaging) {
        m_currentTrait[m_imgName]++;
    }
    // 标注完成Taging状态置成false
    m_isTaging=false;
    qDebug() << tmp;
    return tmp;
}

std::optional<boost::json::value> LabelImgData::praseRespose(const char *response, std::size_t lenth)
{
    boost::json::value v;
    boost::json::stream_parser p;
    boost::system::error_code ec;
    p.write(response, lenth, ec);
    if (ec) {
        qDebug() << "-> parse json failed";
        qDebug() << QString(response);
        return std::nullopt;
    } else {
        v = p.release();
        auto err = v.find_pointer("/eMessage", ec);
        if (!ec) {
            // 请求错误
            qDebug() << err->get_string().c_str();
            return std::nullopt;
        }
    }
    return v;
}
