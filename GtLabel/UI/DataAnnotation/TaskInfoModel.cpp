#include "TaskInfoModel.h"

TaskInfoModel::TaskInfoModel(QObject *parent)
    : QAbstractListModel{parent}
{}

TaskInfoModel::TaskInfoModel(const TaskInfoModel &it) {
    m_datas = it.m_datas;
}

int TaskInfoModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_datas.length();
}

QVariant TaskInfoModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid() || index.row() < 0) return {};
    switch (role) {
    case TaskInfoNameRole:
        return m_datas.at(index.row()).property("taskName");
        break;
    case TaskInfoCreateTimeRole:
        return m_datas.at(index.row()).property("taskNameCreateTime");
        break;
    case TaskInfoCountRole:
        return m_datas.at(index.row()).property("taskImgCount");
        break;
    case TaskInfoLabCountRole:
        return m_datas.at(index.row()).property("taskGetCount");
        break;
    case TaskContinueCountRole:
        return m_datas.at(index.row()).property("taskCompleteCount");
        break;
    case TaskTypeRole:
        return m_datas.at(index.row()).property("dataType");
        break;
    case TaskIdRole:
        return m_datas.at(index.row()).property("taskid");
        break;
    default:
        break;
    }
    return {};
}

QHash<int, QByteArray> TaskInfoModel::roleNames() const {
    return {
        {TaskInfoNameRole,"taskInfoName"},
        {TaskInfoCreateTimeRole,"taskInfoCreateTime"},
        {TaskInfoCountRole,"taskInfoCount"},
        {TaskInfoLabCountRole,"taskInfoLabCount"},
        {TaskContinueCountRole,"taskContinueCount"},
        {TaskTypeRole,"taskType"},
        {TaskIdRole,"taskId"}
        };
}

TaskInfoModel &TaskInfoModel::operator=(const TaskInfoModel &it) {
    return *this;
}

void TaskInfoModel::setDatas(
    QList<std::tuple<QString, QString, unsigned int, unsigned int, unsigned int, unsigned int>> d)
{
    m_datas.clear();
    beginResetModel();
    for(auto& it:d)
        m_datas.append(TaskInfoItem(it));
    endResetModel();
}
