#ifndef LABELIMGDATA_H
#define LABELIMGDATA_H

#include <QObject>
#include <QQmlEngine>
#include <boost/json.hpp>
#include <thread>

#include "TaskInfoModel.h"
#include "TaskInfoItem.h"
#include "HttpClient.h"

class LabelImgData : public QObject
{
    Q_OBJECT

    QML_NAMED_ELEMENT(QmlLabelImgData)

    Q_PROPERTY(QVariant taskInfoModel MEMBER m_model NOTIFY taskInfoModelChanged FINAL)
public:
    explicit LabelImgData(QObject* parent=nullptr);
    ~LabelImgData();
    Q_INVOKABLE void requestImgInfo();
    Q_INVOKABLE void requestImgName(QString name);
signals:
    void taskInfoModelChanged();
private:
    std::optional<boost::json::value> praseRespose(const char *response, std::size_t lenth);
private:
    std::shared_ptr<HttpClient> m_HttpClient;
    boost::asio::io_context m_ioc;
    std::thread m_thd;
    QStringList m_imgNames;
    TaskInfoModel* m_taskInfoModel;
    QVariant m_model;
};

Q_DECLARE_METATYPE(TaskInfoModel)

#endif // LABELIMGDATA_H
