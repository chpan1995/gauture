﻿#include "DatatypeModel.h"

DataNode::DataNode(DataNode *parent):m_parent(parent)
{
    if(parent)
        parent->addChild(this);
}

QString DataNode::tagName()
{
    return m_tagName;
}

void DataNode::setTagName(QString v)
{
    m_tagName = v;
    emit tagNameChanged();
}

int DataNode::deep()
{
    return m_deep;
}

void DataNode::setDeep(int v)
{
    m_deep = v;
    emit deepChanged();
}

QVariantList DataNode::inheritsName()
{
    return m_inheritsName;
}

void DataNode::setInheritsName(QVariantList v)
{
    m_inheritsName = v;
    emit inheritsNameChanged();
}

void getAllTagname(DataNode* node,QVariantList& v){
    if(!node) return;
    v.append(node->tagName());
    return getAllTagname(node->m_parent,v);
}

void DataNode::setInheritsName()
{
    QVariantList v {};
    v.append(m_tagName);
    getAllTagname(m_parent,v);
    setInheritsName(v);
}

int DataNode::getDeep()
{
    std::uint8_t deep=0;
    DataNode* parent = m_parent;
    while(parent) {
        deep++;
        parent = parent->m_parent;
    }
    setDeep(deep);
    return deep;
}

////////////////////////////////////////////////////////

class DatatypeModelPrivate{
public:
    QList<DataNode*> m_data;
    QQmlListProperty<DataNode>* m_treeNodes;
};


static void nodeAppend(QQmlListProperty<DataNode> *prop, DataNode *val)
{
    Q_UNUSED(val);
    Q_UNUSED(prop);
}

static DataNode* nodeAt(QQmlListProperty<DataNode> *prop, qsizetype index)
{
    DatatypeModelPrivate *d = static_cast<DatatypeModelPrivate*>(prop->data);
    return d->m_data.at(index);
}

static qsizetype nodeCount(QQmlListProperty<DataNode> *prop)
{
    DatatypeModelPrivate *d = static_cast<DatatypeModelPrivate*>(prop->data);
    return d->m_data.size();
}

static void nodeClear(QQmlListProperty<DataNode> *prop)
{
    static_cast<DatatypeModelPrivate*>(prop->data)->m_data.clear();
}


DatatypeModel::DatatypeModel(boost::json::value&& data,QObject *parent)
    : QObject{parent},d_ptr{new DatatypeModelPrivate}
{
    d_ptr->m_treeNodes=new QQmlListProperty<DataNode>(this,d_ptr,
        nodeAppend,nodeCount,nodeAt,nodeClear);
    updateData(data);
}

DatatypeModel::~DatatypeModel()
{
    if(d_ptr->m_treeNodes) delete d_ptr->m_treeNodes;
    qDeleteAll(d_ptr->m_data);
}

QQmlListProperty<DataNode> DatatypeModel::treeNodes()
{
    return *d_ptr->m_treeNodes;
}

void recursionData(DatatypeModel* model,boost::json::value& v,DataNode* parent=nullptr) {
    boost::system::error_code ec;
    auto obj = v.find_pointer("/value",ec);
    DataNode* node=nullptr;
    if(!ec && obj->if_string()) {
        node=new DataNode(parent);
        node->setTagName(obj->as_string().c_str());
        node->setInheritsName();
        node->getDeep();
        model->d_ptr->m_data.append(node);
    }

    auto chrid = v.find_pointer("/items",ec);
    if(ec) return;
    if(!chrid->if_array() || chrid->as_array().size()==0)
        return;
    for(auto& it : chrid->as_array()) {
        recursionData(model,it,node);
    }
}

void tagNames(DataNode* node,QStringList& data){
    data.append(node->tagName());
    if(node->m_child.size() <= 0) return;
    for(auto& it:node->m_child) {
        tagNames(it,data);
    }
}

void DatatypeModel::updateData(boost::json::value& v)
{
    recursionData(this,v);
    emit treeNodesChanged();

    if(d_ptr->m_data.size()>0) {
        auto root = d_ptr->m_data[0];
        m_title=root->tagName();
        emit titleChanged();
        QVariantList data1;
        for(auto& it:root->m_child) {
            QStringList data2;
            tagNames(it,data2);
            data1.append(data2);
        }
        m_sortNodes = data1;
        emit sortNodesChanged();
    }
}



AllDatatypeModel::AllDatatypeModel(QObject *parent):QObject(parent)
{
    // test code
    std::string json_str = R"({
        "graintype": "wheat",
        "items": [
            {
                "value": "颗粒类型",
                "items": [
                    {
                        "value": "病斑",
                        "items": [
                            {
                                "value": "赤霉",
                                "items": []
                            },
                            {
                                "value": "黑胚",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "生芽",
                        "items": [
                            {
                                "value": "胡须",
                                "items": []
                            }
                        ]
                    }
                ]
            },
            {
                "value": "自定义类型",
                "items": [
                    {
                        "value": "破损",
                        "items": [
                            {
                                "value": "二分之一",
                                "items": []
                            },
                            {
                                "value": "四分之三",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "热损伤",
                        "items": [
                            {
                                "value": "自然",
                                "items": []
                            }
                        ]
                    }
                ]
            }
        ]
    })";
    praseData(boost::json::parse(json_str));
}

void AllDatatypeModel::praseData(boost::json::value &&v)
{
    for(auto it:m_allDatas){
        auto ptr = it.value<DatatypeModel*>();
        ptr->deleteLater();
    }
    m_allDatas.clear();

    boost::system::error_code ec;
    auto obj = v.find_pointer("/items",ec);
    if(!ec && obj->if_array()) {
        for(auto& it : obj->as_array()) {
            DatatypeModel* node =new DatatypeModel(std::move(it));
            m_allDatas.append(QVariant::fromValue(node));
        }
    }

    emit allDatasChanged();
}
