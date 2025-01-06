#include "DatatypeModel.h"

DataNode::DataNode(DataNode *parent):m_parent(parent)
{
    std::uint8_t deep=0;
    while(m_parent) {
        deep++;
        m_parent=m_parent->m_parent;
    }
    setDeep(deep);
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
    qDebug() << boost::json::serialize(v);
    boost::system::error_code ec;
    auto obj = v.find_pointer("/value",ec);
    DataNode* node=nullptr;
    if(!ec && obj->if_string()) {
        node=new DataNode(parent);
        node->setTagName(obj->as_string().c_str());
        node->setInheritsName();
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

void DatatypeModel::updateData(boost::json::value& v)
{
    recursionData(this,v);
    // DataNode* root=new DataNode();
    // root->setTagName("颗粒类型");
    // root->setDeep(0);
    // d_ptr->m_data.append(root);
    // DataNode* bws=new DataNode(root);
    // bws->setTagName("病斑");
    // bws->setDeep(1);
    // bws->setInheritsName();
    // d_ptr->m_data.append(bws);
    // DataNode* bwsty=new DataNode(bws);
    // bwsty->setTagName("赤霉");
    // bwsty->setDeep(2);
    // bwsty->setInheritsName();
    // d_ptr->m_data.append(bwsty);

    emit treeNodesChanged();
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
