#include "LabelTags.h"
#include "LabelImgData.h"

LabelTags::LabelTags(QObject *parent)
    : QAbstractListModel{parent}
{
    // connect(this,&QAbstractListModel::dataChanged,this,[this](const QModelIndex &topLeft,
    //         const QModelIndex &bottomRight,const QList<int> &roles){
    //     qDebug() << "dataChanged";
    //     auto myparent = static_cast<LabelImgData*>(this->parent());
    //     myparent->updateTags();
    // });
}

void LabelTags::removeRow(QString sapType,QString inherName,int trait,int firstIndex,int secondIndex,bool Btn) {
    if(!m_datas) return;

    auto myparent=  static_cast<LabelImgData*>(parent());

    if(!Btn) {
        // 上面标签点击的
        for(int i=0;i<m_datas->size();i++) {
            if(m_datas->at(i).property("inherName")==inherName
                && m_datas->at(i).property("sapType")==sapType
                && m_datas->at(i).property("trait")==trait) {
                beginRemoveRows(QModelIndex(),i,i);
                m_datas->removeAt(i);
                endRemoveRows();
                break;
            }
        }
        if(trait==myparent->m_currentTrait[myparent->m_imgName] && myparent->m_isTaging) {
            // 清除选中按钮
            emit clearSelectTag(sapType,inherName,firstIndex,secondIndex);
        }else {
            // 删除之前标注的信息
            myparent->updateTags();
        }

    }else {
        // 左边按钮点的
        for(int i=0;i<m_datas->size();i++) {
            if(m_datas->at(i).property("inherName")==inherName
                && m_datas->at(i).property("sapType")==sapType
                && m_datas->at(i).property("trait")==myparent->m_currentTrait[myparent->m_imgName]) {
                beginRemoveRows(QModelIndex(),i,i);
                m_datas->removeAt(i);
                endRemoveRows();
                break;
            }
        }
    }
    bool tmp = false;
    for(auto it:*m_datas)
    {
        // 查找当前特征 对比 历史标签里面有没有，没有则代表当前没有正在打tag
        if(it.property("trait").toInt()==myparent->m_currentTrait[myparent->m_imgName]){
            tmp=true;
            break;
        }
    }
    myparent->m_isTaging=tmp;
}

void LabelTags::appendRow(
    QString sapType, QString inherName, int firstIndex, int secondIndex, QString topName, int trait)
{
    if(!m_datas) return;
    auto myparent=  static_cast<LabelImgData*>(parent());
    for(int i=0;i<m_datas->size();i++) {
        if(m_datas->at(i).property("topName")==topName
            && m_datas->at(i).property("trait")==myparent->m_currentTrait[myparent->m_imgName]
            && m_datas->at(i).property("sapType")==sapType) {
            beginRemoveRows(QModelIndex(),i,i);
            m_datas->removeAt(i);
            endRemoveRows();
            break;
        }
    }
    beginInsertRows(QModelIndex(),m_datas->size(),m_datas->size());
    m_datas->append(LabelTagsItem(sapType,inherName,firstIndex,secondIndex,topName
                                  ,myparent->m_currentTrait[myparent->m_imgName]));
    endInsertRows();
    myparent->m_isTaging=true;
}

void LabelTags::initModel(QList<LabelTagsItem>* d) {
    if(d) {
        beginResetModel();
        m_datas=d;
        endResetModel();
    }
}

void LabelTags::removeRows() {
    if(!m_datas) return;
    auto myparent=  static_cast<LabelImgData*>(parent());
    for (int i = m_datas->size() - 1; i >= 0; i--) {
        if(m_datas->at(i).property("trait")==myparent->m_currentTrait[myparent->m_imgName]) {
            beginRemoveRows(QModelIndex(),i,i);
            m_datas->removeAt(i);
            endRemoveRows();
        }
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

LabelTagsItem::LabelTagsItem(boost::json::value v) {
    boost::system::error_code ec;
    auto sapType = v.find_pointer("/sapType",ec);
    if(!ec)
        m_sapType = sapType->get_string().c_str();

    auto inherName = v.find_pointer("/inherName",ec);
    if(!ec)
        m_inherName = inherName->get_string().c_str();

    auto firstIndex = v.find_pointer("/firstIndex",ec);
    if(!ec)
        m_firstIndex = firstIndex->get_int64();

    auto secondIndex = v.find_pointer("/secondIndex",ec);
    if(!ec)
        m_secondIndex = secondIndex->get_int64();

    auto topName = v.find_pointer("/topName",ec);
    if(!ec)
        m_topName = topName->get_string().c_str();

    auto trait = v.find_pointer("/trait",ec);
    if(!ec)
        m_trait = trait->get_int64();
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

