#ifndef TASKINFOMODEL_H
#define TASKINFOMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "TaskInfoItem.h"


class TaskInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataTypes {
        TaskInfoNameRole = Qt::UserRole+1,
        TaskInfoCreateTimeRole,
        TaskInfoCountRole,
        TaskInfoLabCountRole,
        TaskContinueCountRole
    };
    explicit TaskInfoModel(QObject *parent = nullptr);
    TaskInfoModel(const TaskInfoModel& it);
    TaskInfoModel& operator=(const TaskInfoModel& it);
    void setDatas(QList<std::tuple<QString,QString,
                                   unsigned int,unsigned int,unsigned int>>);
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const  override;
private:

signals:
private:
    QList<TaskInfoItem> m_datas;
};

#endif // TASKINFOMODEL_H
