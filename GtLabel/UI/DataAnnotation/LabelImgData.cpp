#include "LabelImgData.h"
#include "log.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <boost/asio.hpp>
#include "Utils.h"

LabelImgData::LabelImgData(QObject *parent)
    : QObject(parent)
    , m_taskInfoModel(new TaskInfoModel(this))
    , m_labelTags(new LabelTags(this))
{
    m_HttpClient = std::make_shared<HttpClient>(m_ioc);
    m_thd = std::thread([this] {
        auto work(boost::asio::make_work_guard(m_ioc));
        m_ioc.run();
    });
    qRegisterMetaType<LabelImgNamespace::RequestMethod>("RequestMethod");
    connect(this,&LabelImgData::imgNameChanged,this,&LabelImgData::slotImgNamechanged);
    connect(this,&LabelImgData::sigTaskInfoFinished,this,&LabelImgData::slotTaskInfoFinished);
    qApp->installEventFilter(this);
}

LabelImgData::~LabelImgData()
{
    m_ioc.stop();
    if (m_thd.joinable())
        m_thd.join();
}

void LabelImgData::requestImgInfo()
{
    m_das.clear();
    m_HttpClient->addRequest(HttpRequest(
        "192.168.1.158",
        "8080",
        "/api/labTask/labTaskList",
        boost::json::serialize(boost::json::object({{"userid", common::userid}})),
        [this](const char *response, std::size_t lenth) {
            boost::system::error_code ec;
            auto v = praseRespose(response, lenth);
            if (v.has_value()) {
                if (!v->as_object().contains("code"))
                    return;
                if (v->as_object().at("code") != 200)
                    return;
                auto rootobj = v->as_object();
                auto arr = rootobj.at("labtask").as_array();
                logging::log_info(RL,response);
                for (auto &it : arr) {
                    std::string taskName, taskNameCreateTime;
                    unsigned int taskImgCount, taskGetCount, taskCompleteCount, taskid;
                    std::tuple<QString, QString, unsigned int, unsigned int, unsigned int, unsigned int>
                        tp;
                    auto obj = it.as_object();
                    if (obj.contains("imgcnt"))
                        taskGetCount = obj.at("imgcnt").get_int64();
                    if (obj.contains("uploadcnt"))
                        taskCompleteCount = obj.at("uploadcnt").get_int64();
                    if (obj.contains("taskname"))
                        taskName = obj.at("taskname").get_string();
                    if (obj.contains("taskid"))
                        taskid = obj.at("taskid").get_int64();
                    if (obj.contains("createtime"))
                        taskNameCreateTime = obj.at("createtime").get_string();

                    tp = {taskName.c_str(),
                          taskNameCreateTime.c_str(),
                          taskImgCount,
                          taskGetCount,
                          taskCompleteCount,
                          taskid};
                    m_das.append(tp);
                }
            }
        }));

    m_HttpClient->addRequest(HttpRequest(
        "192.168.1.108", "8083", "/tasks/info", [this](const char *response, std::size_t lenth) {
            boost::system::error_code ec;
            auto v = praseRespose(response, lenth);
            if (v.has_value()) {
                emit request(true, LabelImgNamespace::RequestMethod::TasksInfo);
                auto objArry = v.value().find_pointer("/message", ec);
                if (!ec && objArry->is_array()) {
                    QString taskName, taskNameCreateTime;
                    unsigned int taskImgCount;
                    std::tuple<QString, QString, unsigned int, unsigned int, unsigned int, unsigned int>
                        tp;
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
                        tp = {taskName, taskNameCreateTime, taskImgCount, 0, 0, 0};
                        m_das.append(tp);
                    }
                    emit sigTaskInfoFinished();
                }
            } else {
                emit request(false, LabelImgNamespace::RequestMethod::TasksInfo);
            }
        }));
}

void LabelImgData::requestImgName(QString name)
{
    m_currentTaskName = name;
    m_HttpClient->addRequest(
        HttpRequest("192.168.1.108",
                    "8083",
                    QString(QString("/tasks/pull/") + name).toStdString(),
                    [this](const char *response, std::size_t lenth) {
                        auto v = praseRespose(response, lenth);
                        if (v.has_value()) {
                            emit request(true, LabelImgNamespace::RequestMethod::TasksPull);
                            m_imgNames.clear();
                            m_labelTagsModels.clear();
                            boost::system::error_code ec;
                            auto objArry = v.value().find_pointer("/message", ec);
                            if (!ec && objArry->is_array()) {
                                for (auto &it : objArry->as_array()) {
                                    if (it.is_string()) {
                                        m_imgNames.append(it.get_string().c_str());
                                    }
                                }
                            }
                            for (auto &it : m_imgNames) {
                                m_labelTagsModels.insert(it, QList<LabelTagsItem>());
                                m_currentTrait.insert(it, 1);
                            }
                            if (m_imgNames.size() > 0) {
                                m_imgName = m_imgNames[0];
                                emit imgNameChanged();
                                // 线程更新model竟然不刷新？？？？？？？
                                // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
                            }
                            m_allPage = m_imgNames.size();
                            m_currentPage = 0;
                            emit currentPageChanged();
                            emit allPageChanged();
                            m_HttpClient->addRequest(HttpRequest(
                                "192.168.1.158",
                                "8080",
                                "/api/labTask/addLabTask",
                                boost::json::serialize(boost::json::object(
                                    {{"userid", 1},
                                     {"taskname", m_currentTaskName.toStdString()},
                                     {"imgarry", objArry->get_array()}})),
                                [this](const char *response, std::size_t lenth) {
                                    auto v = praseRespose(response, lenth);
                                    if (v.has_value()) {
                                        logging::log_info(RL, boost::json::serialize(v.value()));
                                        boost::system::error_code ec;
                                        auto taskid = v.value().find_pointer("/labTaskid", ec);
                                        if (!ec && taskid->if_int64()) {
                                            m_currentTaskId = taskid->get_int64();
                                        }
                                    }
                                }));
                        } else {
                            emit request(false, LabelImgNamespace::RequestMethod::TasksPull);
                        }
                    }));
}

void LabelImgData::requestImgName(QString name, int taskid)
{
    m_currentTaskId = taskid;
    m_currentTaskName = name;
    m_HttpClient->addRequest(
        HttpRequest("192.168.1.158",
                    "8080",
                    QString("/api/labTask/labTaskInfo").toStdString(),
                    boost::json::serialize(boost::json::object({{"taskid", taskid}})),
                    [this](const char *response, std::size_t lenth) {
                        auto v = praseRespose(response, lenth);
                        if (v.has_value()) {
                            if (!v->as_object().contains("code")) {
                                emit request(false, LabelImgNamespace::RequestMethod::TasksPull);
                                return;
                            }
                            if (v->as_object().at("code") != 200) {
                                emit request(false, LabelImgNamespace::RequestMethod::TasksPull);
                                return;
                            }
                            emit request(true, LabelImgNamespace::RequestMethod::TasksPull);
                            m_imgNames.clear();
                            m_labelTagsModels.clear();
                            QList<QString> tags;
                            boost::system::error_code ec;
                            auto objArry = v.value().find_pointer("/labtaskinfo", ec);
                            if (!ec && objArry->is_array()) {
                                for (auto &it : objArry->as_array()) {
                                    if (it.is_object()) {
                                        auto imgobj = it.find_pointer("/imgname", ec);
                                        if (!ec) {
                                            m_imgNames.append(imgobj->get_string().c_str());
                                        }
                                        auto tagsobj = it.find_pointer("/tags", ec);
                                        if (!ec) {
                                            tags.append(tagsobj->get_string().c_str());
                                        }
                                    }
                                }
                            }
                            for (int i = 0; i < m_imgNames.size(); i++) {
                                try {
                                    QList<LabelTagsItem> da;
                                    auto value = boost::json::parse(tags[i].toStdString());
                                    if (value.is_array()) {
                                        for (auto &it : value.get_array()) {
                                            da.append(LabelTagsItem(it.get_object()));
                                        }
                                    }
                                    m_labelTagsModels.insert(m_imgNames[i], da);
                                    if(da.size() > 0) {
                                        m_currentTrait.insert(m_imgNames[i],
                                                              da[da.size()-1].property("trait").toInt() + 1);
                                    }else {
                                        m_currentTrait.insert(m_imgNames[i],1);
                                    }
                                } catch (boost::system::system_error& ec) {
                                }
                            }
                            if (m_imgNames.size() > 0) {
                                m_imgName = m_imgNames[0];
                                emit imgNameChanged();
                                // 线程更新model竟然不刷新？？？？？？？
                                // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
                            }
                            m_allPage = m_imgNames.size();
                            m_currentPage = 0;
                            emit currentPageChanged();
                            emit allPageChanged();
                        } else {
                            emit request(false, LabelImgNamespace::RequestMethod::TasksPull);
                        }
                    }));
}

bool LabelImgData::gotoImgs(LabelImgNamespace::PageGo v)
{
    // m_HttpClient->addRequest(
    //     HttpRequest("192.168.1.158",
    //                 "8080",
    //                 QString(QString("/api/user/login")).toStdString(),
    //                 boost::json::serialize(boost::json::object({{"username","fdd"},{"password","123456"}})),
    //                 [this](const char *response, std::size_t lenth) {
    //                     auto v = praseRespose(response, lenth);
    //                     if(v.has_value()) {
    //                         qDebug() <<  boost::json::serialize(v.value());
    //                     }
    //                 }));
    // 清除未点击标注信息
    if (m_isTaging) {
        return false;
    }
    switch (v) {
    case LabelImgNamespace::PageGo::Next: {
        int index = ++m_currentPage;
        if (index > m_imgNames.size() - 1) {
            m_currentPage--;
        } else {
            m_imgName = m_imgNames[index];
            emit imgNameChanged();
            // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
            m_isTaging = false;
        }
    } break;
    case LabelImgNamespace::PageGo::Front: {
        int index = --m_currentPage;
        if (index < 0) {
            m_currentPage++;
        } else {
            m_imgName = m_imgNames[index];
            emit imgNameChanged();
            // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
            m_isTaging = false;
        }
    } break;
    case LabelImgNamespace::PageGo::Tail: {
        m_currentPage = 0;
        if (m_imgNames.size() > 0) {
            m_imgName = m_imgNames[m_currentPage];
            emit imgNameChanged();
            // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
        }
        m_isTaging = false;
    } break;
    case LabelImgNamespace::PageGo::End: {
        m_currentPage = m_imgNames.size() - 1;
        if (m_imgNames.size() > 0) {
            m_imgName = m_imgNames[m_currentPage];
            emit imgNameChanged();
            // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
        }
        m_isTaging = false;
    } break;
    case LabelImgNamespace::PageGo::Front10: {
        if (m_currentPage - 10 < 0) {
            m_currentPage = 0;
        } else {
            m_currentPage = m_currentPage - 10;
        }
        if (m_imgNames.size() > 0) {
            m_imgName = m_imgNames[m_currentPage];
            emit imgNameChanged();
            // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
        }
        m_isTaging = false;
    } break;
    case LabelImgNamespace::PageGo::Next10: {
        if (m_currentPage + 10 > m_imgNames.size() - 1) {
            m_currentPage = m_imgNames.size() - 1;
        } else {
            m_currentPage = m_currentPage + 10;
        }
        if (m_imgNames.size() > 0) {
            m_imgName = m_imgNames[m_currentPage];
            emit imgNameChanged();
            // m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
        }
        m_isTaging = false;
    } break;
    default:
        break;
    }
    emit currentPageChanged();
    return true;
}

void LabelImgData::setTagStatus(bool f)
{
    m_isTaging = f;
}

void LabelImgData::reset()
{
    m_isTaging = false;
    if (m_labelTagsModels.contains(m_imgName)) {
        m_currentTrait[m_imgName] = 1;
        m_labelTagsModels[m_imgName].clear();
        m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
        updateTags();
    }
}

bool LabelImgData::lab()
{
    bool tmp = m_isTaging;
    if (m_isTaging) {
        m_currentTrait[m_imgName]++;
        updateTags(); // 更新tags
    }
    // 标注完成Taging状态置成false
    m_isTaging = false;
    return tmp;
}

QVariantList LabelImgData::upload()
{
    if (m_isTaging) {
        logging::log_info(RL, "{}", "存在未标注信息,请标注完或取消标注在上传");
        return {false, "存在未确认的标注信息,请点击标注完或取消标注在上传"};
    }
    boost::json::value v{{"taskName", m_currentTaskName.toStdString()},
                         {"userName", common::username},
                         {"info", boost::json::array()}};

    boost::json::array tagarr;
    for (auto &it : m_labelTagsModels.keys()) {
        if (m_labelTagsModels[it].size() > 0) {
            boost::json::object obj;
            obj.insert({{"imageName", it.toStdString()}});
            obj.insert({{"grainType",
                         m_labelTagsModels[it][0].property("sapType").toString().toStdString()}});
            boost::json::array arr;
            int currentTrait, oldTrait = -1;
            boost::json::array arrChird;
            boost::json::array tagArrChird;
            for (int i = 0; i < m_labelTagsModels[it].size(); i++) {
                currentTrait = m_labelTagsModels[it][i].property("trait").toInt();
                if (currentTrait != oldTrait && oldTrait != -1) {
                    arr.emplace_back(arrChird);
                    arrChird.clear();
                    arrChird.emplace_back(
                        m_labelTagsModels[it][i].property("inherName").toString().toStdString());
                } else {
                    arrChird.emplace_back(
                        m_labelTagsModels[it][i].property("inherName").toString().toStdString());
                }
                oldTrait = currentTrait;
                if (i == m_labelTagsModels[it].size() - 1) {
                    arr.emplace_back(arrChird);
                }
                arr.push_back(obj);
                /////////////////////////////////////////////////////////
                boost::json::object obj({{"sapType", m_labelTagsModels[it][i].property("sapType").toString().toStdString()},
                                         {"inherName", m_labelTagsModels[it][i].property("inherName").toString().toStdString()},
                                         {"firstIndex", m_labelTagsModels[it][i].property("firstIndex").toInt()},
                                         {"secondIndex", m_labelTagsModels[it][i].property("sapType").toInt()},
                                         {"topName", m_labelTagsModels[it][i].property("topName").toString().toStdString()},
                                         {"trait", m_labelTagsModels[it][i].property("trait").toInt()}});
                tagArrChird.emplace_back(obj);
            }
            tagarr.emplace_back(boost::json::array({m_currentTaskId,it.toStdString(),boost::json::serialize(tagArrChird)}));
            obj.insert({{"property", arr}});
            v.at("info").as_array().emplace_back(obj);
        }
    }

    int imgCount = v.at("info").as_array().size();
    if (imgCount == 0) {
        logging::log_info(RL, "{}", "未标注任何图片");
        return {false, "未标注任何图片,禁止上传"};
    }

    // test code
    std::stringstream os;
    std::string indent = "";
    pretty_print(os, v, &indent);
    QString timestamp = QDateTime::currentDateTime().toString(
        "yyyy-MM-dd_hh-mm-ss"); // 使用下划线和连字符替换空格和冒号
    QString fpath = QCoreApplication::applicationDirPath() + "/" + timestamp + ".json";

    // // 2. 确保目录存在（注意：这里要传入目录路径，而不是文件路径）
    // QFileInfo fileInfo(fpath);
    // QDir dir(fileInfo.absolutePath());
    // if (!dir.exists()) {
    //     if (!dir.mkpath(".")) { // 创建到当前目录的路径
    //         logging::log_error(RL, "创建目录失败");
    //     }
    // }
    QFile File(fpath);
    if (!File.open(QFile::ReadWrite | QFile::Text | QFile::Truncate)) {
        logging::log_error(RL, "打开文件失败{}", File.errorString().toStdString());
    } else {
        File.write(os.str().c_str());
        File.close();
    }
    m_HttpClient->addRequest(HttpRequest(
        "192.168.1.108",
        "8083",
        "/tasks/push/" + m_currentTaskName.toStdString(),
        boost::json::serialize(v),
        [this, imgCount,tagarr](const char *response, std::size_t lenth) {
            boost::system::error_code ec;
            auto v = praseRespose(response, lenth);
            if (v.has_value()) {
                boost::system::error_code ec;
                auto code = v.value().find_pointer("/code", ec);
                if (!ec && code->is_int64()) {
                    if (code->as_int64() == 200) {
                        QString content
                            = QString("上传成功，本次获取任务图片共：%1张，上传标注图片%2张")
                                  .arg(m_imgNames.size())
                                  .arg(imgCount);
                        auto failed_count = v.value().find_pointer("/failed_count", ec);
                        if (!ec && code->is_int64()) {
                            if (failed_count->as_int64() > 0) {
                                content.append(QString("，失败%1张").arg(failed_count->as_int64()));
                            }
                        }
                        emit request(true, LabelImgNamespace::RequestMethod::TasksPush, content);
                        m_HttpClient->addRequest(
                            HttpRequest("192.168.1.158", "8080", "/api/labTask/labupload/"
                                        ,boost::json::serialize(tagarr), [this](const char *response, std::size_t lenth) {
                                auto v = praseRespose(response, lenth);
                                if (v.has_value()) {
                                    logging::log_info(RL, boost::json::serialize(v.value()));
                                }
                            }));
                    }
                }
            } else {
                emit request(false, LabelImgNamespace::RequestMethod::TasksPush);
            }
        }));
    return {true, "正在上传中..."};
}

void LabelImgData::clear()
{
    QList<LabelTagsItem> tmp{};
    m_labelTags->initModel(&tmp);

    m_allPage = 0;
    m_currentPage = 0;
    m_imgNames.clear();
    m_imgName = "";
    m_labelTagsModels.clear();
    emit imgNameChanged();
    emit currentPageChanged();
    emit allPageChanged();
}

void LabelImgData::slotImgNamechanged() {
    if(m_imgName!=""){
        m_labelTags->initModel(&m_labelTagsModels[m_imgName]);
    }
}

void LabelImgData::slotTaskInfoFinished() {
    m_taskInfoModel->setDatas(m_das);
    m_model = QVariant::fromValue(m_taskInfoModel);
    emit taskInfoModelChanged();
}

bool LabelImgData::eventFilter(QObject *object, QEvent *event) {
    if(event->type() == NetState::TYPE) {
        auto net = static_cast<NetState*>(event);
        emit netState(net->state());
    }
    return QObject::eventFilter(object, event);
}

void LabelImgData::updateTags()
{
    boost::json::array arr;
    for (auto it : m_labelTagsModels[m_imgName.toStdString().c_str()]) {
        boost::json::object obj({{"sapType", it.property("sapType").toString().toStdString()},
                                 {"inherName", it.property("inherName").toString().toStdString()},
                                 {"firstIndex", it.property("firstIndex").toInt()},
                                 {"secondIndex", it.property("sapType").toInt()},
                                 {"topName", it.property("topName").toString().toStdString()},
                                 {"trait", it.property("trait").toInt()}});
        arr.push_back(obj);
    }
    boost::json::object v({{"taskid", m_currentTaskId},
                           {"imgname", m_imgName.toStdString()},
                           {"tags", boost::json::serialize(arr)}});
    m_HttpClient->addRequest(
        HttpRequest("192.168.1.158",
                    "8080",
                    "/api/labTask/updateTags",
                    boost::json::serialize(v),
                    [this](const char *response, std::size_t lenth) {
                        auto v = praseRespose(response, lenth);
                        if (v.has_value()) {
                            logging::log_info(RL, boost::json::serialize(v.value()));
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
        logging::log_info(RL, "{}--{}", "parse json failed", response);
        return std::nullopt;
    } else {
        v = p.release();
        auto err = v.find_pointer("/eMessage", ec);
        if (!ec) {
            // 请求错误
            logging::log_info(RL, "请求错误:{}{}", "parse json failed", err->get_string().c_str());
            return std::nullopt;
        }
    }
    return v;
}
