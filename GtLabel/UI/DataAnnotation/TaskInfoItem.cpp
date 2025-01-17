#include "TaskInfoItem.h"

TaskInfoItem::TaskInfoItem(QObject *parent)
    : QObject{parent}
{}

TaskInfoItem::TaskInfoItem(const TaskInfoItem &it) {
    m_taskName = it.m_taskName;
    m_taskNameCreateTime = it.m_taskNameCreateTime;
    m_taskImgCount = it.m_taskImgCount;
    m_taskGetCount = it.m_taskGetCount;
    m_taskCompleteCount = it.m_taskCompleteCount;
}

TaskInfoItem::TaskInfoItem(
    std::tuple<QString, QString, unsigned int, unsigned int, unsigned int> data, QObject *parent)
    : QObject{parent}
{
    auto &[taskName,taskNameCreateTime,taskImgCount,taskGetCount,taskCompleteCount] = data;
    m_taskName = taskName;
    m_taskNameCreateTime = taskNameCreateTime;
    m_taskImgCount = taskImgCount;
    m_taskGetCount = taskGetCount;
    m_taskCompleteCount = taskCompleteCount;
    emit taskNameChanged();
    emit taskNameCreateTimeChanged();
    emit taskImgCountChanged();
    emit taskGetCountChanged();
    emit taskCompleteCountChanged();
}

TaskInfoItem& TaskInfoItem::operator=(const TaskInfoItem &it) {
    m_taskName = it.m_taskName;
    m_taskNameCreateTime = it.m_taskNameCreateTime;
    m_taskImgCount = it.m_taskImgCount;
    m_taskGetCount = it.m_taskGetCount;
    m_taskCompleteCount = it.m_taskCompleteCount;
    return *this;
}
