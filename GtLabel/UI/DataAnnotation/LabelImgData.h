#ifndef LABELIMGDATA_H
#define LABELIMGDATA_H

#include <QObject>
#include <QQmlEngine>
#include <boost/json.hpp>
#include <thread>

#include "Utils.h"
#include "LabelTags.h"
#include "TaskInfoModel.h"
#include "HttpClient.h"

namespace LabelImgNamespace {
Q_NAMESPACE
QML_ELEMENT
enum class RequestMethod {
    TasksInfo,
    TasksPull,
    TasksPush
};
enum class PageGo {
    Tail,
    Front10,
    Front,
    Next,
    Next10,
    End
};

Q_ENUM_NS(RequestMethod)
Q_ENUM_NS(PageGo)
}

class LabelImgData : public QObject
{
    Q_OBJECT

    QML_NAMED_ELEMENT(QmlLabelImgData)

    Q_PROPERTY(QVariant taskInfoModel MEMBER m_model NOTIFY taskInfoModelChanged FINAL)
    Q_PROPERTY(QString imgName MEMBER m_imgName NOTIFY imgNameChanged FINAL)
    Q_PROPERTY(int currentPage MEMBER m_currentPage  NOTIFY currentPageChanged FINAL)
    Q_PROPERTY(int allPage MEMBER m_allPage NOTIFY allPageChanged FINAL)
public:

    explicit LabelImgData(QObject* parent=nullptr);
    ~LabelImgData();
    Q_INVOKABLE void requestImgInfo();
    Q_INVOKABLE void requestImgName(QString name);
    Q_INVOKABLE void requestImgName(QString name,int taskid);
    Q_INVOKABLE bool gotoImgs(LabelImgNamespace::PageGo v);
    Q_INVOKABLE void setTagStatus(bool f);

    // 重置
    Q_INVOKABLE void reset();

    // 标注
    Q_INVOKABLE bool lab();

    // 上传数据
    Q_INVOKABLE QVariantList upload();

    // 清空
    Q_INVOKABLE void clear();

    inline Q_INVOKABLE LabelTags* getLabelTags() { return m_labelTags; }
protected:
    bool eventFilter(QObject *object, QEvent *event) override;
signals:
    void taskInfoModelChanged();
    void imgNameChanged();
    void request(bool v,LabelImgNamespace::RequestMethod method,QString s="");

    void currentPageChanged();
    void allPageChanged();

    void netState(bool v);
private:
    void updateTags();
    std::optional<boost::json::value> praseRespose(const char *response, std::size_t lenth);
private:
    std::shared_ptr<HttpClient> m_HttpClient;
    boost::asio::io_context m_ioc;
    std::thread m_thd;
    QStringList m_imgNames;
    QString m_imgName;
    TaskInfoModel* m_taskInfoModel;
    QVariant m_model;
    qint32 m_currentPage {0};
    qint32 m_allPage {0};
    LabelTags* m_labelTags;
    QHash<QString,QList<LabelTagsItem>> m_labelTagsModels;
    QHash<QString,std::uint16_t> m_currentTrait;
    QString m_currentTaskName;
    std::int32_t m_currentTaskId;
    bool m_isTaging { false }; // 是否正在标记 (打了tag但是没点标注 关闭标签结合trait就可以控制按钮的选中状态了)
    QList<std::tuple<QString, QString, unsigned int, unsigned int, unsigned int, unsigned int>> m_das;
    friend class LabelTags;
};

Q_DECLARE_METATYPE(TaskInfoModel)

#endif // LABELIMGDATA_H
