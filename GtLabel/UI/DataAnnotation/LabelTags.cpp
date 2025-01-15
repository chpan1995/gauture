#include "LabelTags.h"
#include "LabelImgData.h"

LabelTags::LabelTags(QObject *parent)
    : QAbstractListModel{parent}
{}

void LabelTags::removeRow(QString sapType,QString inherName,int trait) {
    if(!m_datas) return;
    for(int i=0;i<m_datas->size();i++) {
        if(m_datas->at(i).property("inherName")==inherName && m_datas->at(i).property("sapType")==sapType) {
            beginRemoveRows(QModelIndex(),i,i);
            m_datas->removeAt(i);
            endRemoveRows();
            break;
        }
    }
    auto myparent=  static_cast<LabelImgData*>(parent());
    bool is_tag {false};
    std::find_if(m_datas->begin(),m_datas->end(),[&](LabelTagsItem it){
        if(it.property("trait").toInt()==myparent->m_currentTrait){
            is_tag=true;
            return true;
        }else {
            is_tag=false;
            return false;
        }
    });
    myparent->setTagStatus(is_tag);
}

void LabelTags::appendRow(
    QString sapType, QString inherName, int firstIndex, int secondIndex, QString topName, int trait)
{
    if(!m_datas) return;
    for(int i=0;i<m_datas->size();i++) {
        if(m_datas->at(i).property("topName")==topName && m_datas->at(i).property("sapType")==sapType) {
            beginRemoveRows(QModelIndex(),i,i);
            m_datas->removeAt(i);
            endRemoveRows();
            break;
        }
    }
    auto myparent=  static_cast<LabelImgData*>(parent());
    beginInsertRows(QModelIndex(),m_datas->size(),m_datas->size());
    m_datas->append(LabelTagsItem(sapType,inherName,firstIndex,secondIndex,topName,myparent->m_currentTrait));
    endInsertRows();
    myparent->setTagStatus(true);
}

void LabelTags::initModel(QList<LabelTagsItem>* d) {
    if(d) {
        beginResetModel();
        m_datas=d;
        endResetModel();
    }
}

int LabelTags::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    if(!m_datas) return 0;
    return m_datas->length();
}

QVariant LabelTags::data(const QModelIndex &index, int role) const {
    if(!index.isValid() || index.row() < 0) return {};
    switch (role) {
    case SapTypeRole:
        return m_datas->at(index.row()).property("sapType");
        break;
    case InherNameRole:
        return m_datas->at(index.row()).property("inherName");
        break;
    case FirstIndexRole:
        return m_datas->at(index.row()).property("firstIndex");
        break;
    case SecondIndexRole:
        return m_datas->at(index.row()).property("secondIndex");
        break;
    case TopNameRole:
        return m_datas->at(index.row()).property("topName");
        break;
    case TraitRole:
        return m_datas->at(index.row()).property("trait");
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

