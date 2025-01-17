#ifndef TASKINFOITEM_H
#define TASKINFOITEM_H

#include <QQmlEngine>
#include <QObject>

class TaskInfoItem : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("");

    Q_PROPERTY(QString taskName MEMBER m_taskName NOTIFY taskNameChanged FINAL)
    Q_PROPERTY(QString taskNameCreateTime MEMBER m_taskNameCreateTime NOTIFY taskNameCreateTimeChanged FINAL)
    Q_PROPERTY(unsigned int taskImgCount MEMBER m_taskImgCount NOTIFY taskImgCountChanged FINAL)

    Q_PROPERTY(unsigned int taskGetCount MEMBER m_taskGetCount NOTIFY taskGetCountChanged FINAL)
    Q_PROPERTY(unsigned int taskCompleteCount MEMBER m_taskCompleteCount NOTIFY taskCompleteCountChanged FINAL)

    Q_PROPERTY(DataTypes m_dataType MEMBER m_dataType NOTIFY dataTypeChanged FINAL)
public:
    enum DataTypes {
        TaskInfoRole = Qt::UserRole+1,
        TaskContinueRole
    };
    Q_ENUM(DataTypes);
    explicit TaskInfoItem(QObject *parent = nullptr);
    TaskInfoItem(const TaskInfoItem& it);
    explicit TaskInfoItem(std::tuple<QString,QString,
                unsigned int,unsigned int,unsigned int> data,QObject *parent = nullptr);
    TaskInfoItem& operator=(const TaskInfoItem& item);
signals:
    void taskNameChanged();
    void taskNameCreateTimeChanged();
    void taskImgCountChanged();
    void taskGetCountChanged();
    void taskCompleteCountChanged();
    void dataTypeChanged();
private:
    DataTypes m_dataType {TaskInfoRole};
    QString m_taskName;
    QString m_taskNameCreateTime;
    unsigned int m_taskImgCount;
    unsigned int m_taskGetCount;
    unsigned int m_taskCompleteCount;
};

Q_DECLARE_METATYPE(TaskInfoItem)

#endif // TASKINFOITEM_H
