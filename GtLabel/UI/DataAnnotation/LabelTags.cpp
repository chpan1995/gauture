#include "LabelTags.h"

LabelTags::LabelTags(QObject *parent)
    : QAbstractListModel{parent}
{}

void LabelTags::removeRow(int row) {
    beginRemoveRows(QModelIndex(),row,row);
    m_datas.removeAt(row);
    endRemoveRows();
}

void LabelTags::appendRow(
    QString sapType, QString inherName, int firstIndex, int secondIndex, QString topName, int trait)
{
    beginInsertRows(QModelIndex(),m_datas.size(),m_datas.size());
    m_datas.append(LabelTagsItem(sapType,inherName,firstIndex,secondIndex,topName,trait));
    endInsertRows();
}

int LabelTags::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_datas.length();
}

QVariant LabelTags::data(const QModelIndex &index, int role) const {
    if(!index.isValid() || index.row() < 0) return {};
    switch (role) {
    case SapTypeRole:
        return m_datas.at(index.row()).property("sapType");
        break;
    case InherNameRole:
        return m_datas.at(index.row()).property("inherName");
        break;
    case FirstIndexRole:
        return m_datas.at(index.row()).property("firstIndex");
        break;
    case SecondIndexRole:
        return m_datas.at(index.row()).property("secondIndex");
        break;
    case TopNameRole:
        return m_datas.at(index.row()).property("topName");
        break;
    case TraitRole:
        return m_datas.at(index.row()).property("trait");
        break;
    default:
        break;
    }
    return {};
}

QHash<int, QByteArray> LabelTags::roleNames() const {
    return {
        {SapTypeRole,"sapType"},
        {InherNameRole,"inherName"},
        {FirstIndexRole,"firstIndex"},
        {SecondIndexRole,"secondIndex"},
        {TopNameRole,"topName"},
        {TraitRole,"trait"}
    };
}

LabelTagsItem::LabelTagsItem(QObject *parent): QObject{parent} {}

LabelTagsItem::LabelTagsItem(
    QString sapType, QString inherName, int firstIndex, int secondIndex, QString topName, int trait)
{
    m_sapType = sapType;
    m_inherName = inherName;
    m_firstIndex = firstIndex;
    m_secondIndex = secondIndex;
    m_topName = topName;
    m_trait = trait;
}

LabelTagsItem::LabelTagsItem(const LabelTagsItem &it) {
    m_sapType = it.m_sapType;
    m_inherName = it.m_inherName;
    m_firstIndex = it.m_firstIndex;
    m_secondIndex = it.m_secondIndex;
    m_topName = it.m_topName;
    m_trait = it.m_trait;
}

LabelTagsItem &LabelTagsItem::operator=(const LabelTagsItem &it) {
    m_sapType = it.m_sapType;
    m_inherName = it.m_inherName;
    m_firstIndex = it.m_firstIndex;
    m_secondIndex = it.m_secondIndex;
    m_topName = it.m_topName;
    m_trait = it.m_trait;
    return *this;
}

LabelTagsItem &LabelTagsItem::operator=(LabelTagsItem &&it) {
    m_sapType = it.m_sapType;
    m_inherName = it.m_inherName;
    m_firstIndex = it.m_firstIndex;
    m_secondIndex = it.m_secondIndex;
    m_topName = it.m_topName;
    m_trait = it.m_trait;
    return *this;
}

