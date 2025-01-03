#include "DatatypeModel.h"

DataNode::DataNode(DataNode *parent):m_parent(parent)
{

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


DatatypeModel::DatatypeModel(QObject *parent)
    : QObject{parent},d_ptr{new DatatypeModelPrivate}
{
    d_ptr->m_treeNodes=new QQmlListProperty<DataNode>(this,d_ptr,
        nodeAppend,nodeCount,nodeAt,nodeClear);
    updateData();
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

void DatatypeModel::updateData()
{
    DataNode* root=new DataNode();
    root->setTagName("颗粒类型");
    root->setDeep(0);
    DataNode* bws=new DataNode(root);
    bws->setTagName("病斑");
    bws->setDeep(1);
    bws->setInheritsName();
    d_ptr->m_data.append(bws);

    emit treeNodesChanged();
}


