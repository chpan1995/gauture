#include "DatatypeModel.h"
#include "Utils.h"

#include <QRegularExpression>

DataNode::DataNode(DataNode *parent) : m_parent(parent)
{
    if (parent)
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

void getAllTagname(DataNode *node, QVariantList &v)
{
    if (!node)
        return;
    v.append(node->tagName());
    return getAllTagname(node->m_parent, v);
}

void DataNode::setInheritsName()
{
    QVariantList v{};
    v.append(m_tagName);
    getAllTagname(m_parent, v);
    setInheritsName(v);
}

int DataNode::getDeep()
{
    std::uint8_t deep = 0;
    DataNode *parent = m_parent;
    while (parent)
    {
        deep++;
        parent = parent->m_parent;
    }
    setDeep(deep);
    return deep;
}

bool DataNode::selected()
{
    return m_selected;
}

void DataNode::setSelected(bool v)
{
    m_selected = v;
    emit selectedChanged();
}

bool DataNode::visiable()
{
    return m_visiable;
}

void DataNode::setVisiable(bool v)
{
    m_visiable = v;
    emit visiableChanged();
}

void setNodesele(DataNode *node)
{
    node->setSelected(false);
    if (node->m_child.size() <= 0)
        return;
    for (auto &it : node->m_child)
    {
        setNodesele(it);
    }
}


void DataNode::qmlSelected(bool v)
{
    DataNode* root = nullptr;
    DataNode* tmp=m_parent;
    while(tmp) {
        if(tmp->m_parent)
            tmp=tmp->m_parent;
        else break;
    }
    if(!m_parent) root=this;
    else root=tmp;
    setNodesele(root);
    setSelected(v);
}

////////////////////////////////////////////////////////

class DatatypeModelPrivate
{
public:
    QList<DataNode *> m_data;
    QQmlListProperty<DataNode> *m_treeNodes;
};

static void nodeAppend(QQmlListProperty<DataNode> *prop, DataNode *val)
{
    Q_UNUSED(val);
    Q_UNUSED(prop);
}

static DataNode *nodeAt(QQmlListProperty<DataNode> *prop, qsizetype index)
{
    DatatypeModelPrivate *d = static_cast<DatatypeModelPrivate *>(prop->data);
    return d->m_data.at(index);
}

static qsizetype nodeCount(QQmlListProperty<DataNode> *prop)
{
    DatatypeModelPrivate *d = static_cast<DatatypeModelPrivate *>(prop->data);
    return d->m_data.size();
}

static void nodeClear(QQmlListProperty<DataNode> *prop)
{
    static_cast<DatatypeModelPrivate *>(prop->data)->m_data.clear();
}

DatatypeModel::DatatypeModel(boost::json::value &&data, QObject *parent)
    : QObject{parent}, d_ptr{new DatatypeModelPrivate}
{
    d_ptr->m_treeNodes = new QQmlListProperty<DataNode>(this, d_ptr,
                                                        nodeAppend, nodeCount, nodeAt, nodeClear);
    updateData(data);
}

DatatypeModel::~DatatypeModel()
{
    if (d_ptr->m_treeNodes)
        delete d_ptr->m_treeNodes;
    qDeleteAll(d_ptr->m_data);
}

QQmlListProperty<DataNode> DatatypeModel::treeNodes()
{
    return *d_ptr->m_treeNodes;
}

void recursionData(DatatypeModel *model, boost::json::value &v, DataNode *parent = nullptr)
{
    boost::system::error_code ec;
    auto obj = v.find_pointer("/value", ec);
    DataNode *node = nullptr;
    if (!ec && obj->if_string())
    {
        node = new DataNode(parent);
        node->setTagName(obj->as_string().c_str());
        node->setInheritsName();
        node->getDeep();
        model->d_ptr->m_data.append(node);
    }

    auto chrid = v.find_pointer("/items", ec);
    if (ec)
        return;
    if (!chrid->if_array() || chrid->as_array().size() == 0)
        return;
    for (auto &it : chrid->as_array())
    {
        recursionData(model, it, node);
    }
}

void tagNames(DataNode *node, QStringList &data)
{
    QString inheritsName = {};
    for (int i = node->inheritsName().size() - 1; i >= 0; i--)
    {
        inheritsName.append(node->inheritsName().at(i).toString());
        if (i != 0)
            inheritsName.append("-");
    }
    data.append(node->tagName() + "," + inheritsName + ",false"); // false 记录选中状态
    if (node->m_child.size() <= 0)
        return;
    for (auto &it : node->m_child)
    {
        tagNames(it, data);
    }
}

void DatatypeModel::updateData(boost::json::value &v)
{
    recursionData(this, v);
    emit treeNodesChanged();

    if (d_ptr->m_data.size() > 0)
    {
        auto root = d_ptr->m_data[0];
        m_title.insert("title", root->tagName());
        m_title.insert("fold", true);
        emit titleChanged();
        QVariantList data1;
        for (auto &it : root->m_child)
        {
            QStringList data2;
            tagNames(it, data2);
            data1.append(data2);
        }
        m_sortNodes = data1;
        emit sortNodesChanged();
    }
}

void DatatypeModel::setSelected(int parentIndexint, int index, QString v)
{
    if (m_sortNodes.size() > parentIndexint)
    {
        QStringList node = m_sortNodes.at(parentIndexint).toStringList();
        QString originalStr = node.at(index);
        // 正则表达式：匹配第二个逗号及其后面的内容
        QRegularExpression regex("(,[^,]*),.*");
        // 替换匹配的部分
        QString resultStr = originalStr.replace(regex, "\\1," + v);
        node.replace(index, resultStr);
        m_sortNodes.replace(parentIndexint, node);

        // 更改非index下的选中状态为false  //相同的parentIndexint
        for(int i=0;i<node.length();i++){
            if(i==index) continue;
            QString originalStr=node[i];
            QRegularExpression regex("(,[^,]*),.*");
            QString resultStr = originalStr.replace(regex, "\\1," + QString("false"));
            node.replace(i, resultStr);
            m_sortNodes.replace(parentIndexint, node);
        }
        for(int i=0;i<m_sortNodes.size();i++) {  //不同的parentIndexint
            if(i==parentIndexint) continue;
            QStringList node = m_sortNodes.at(i).toStringList();
            for(int j=0;j<node.length();j++){
                QString originalStr=node[j];
                QRegularExpression regex("(,[^,]*),.*");
                QString resultStr = originalStr.replace(regex, "\\1," + QString("false"));
                node.replace(j, resultStr);
            }
            m_sortNodes.replace(i, node);
        }
        emit sortNodesChanged();
    }
}

void nodeVisiable(DataNode *node, bool visiable)
{
    if (!node)
        return;
    node->setVisiable(visiable);
    for (auto &it : node->m_child)
    {
        nodeVisiable(it, visiable);
    }
}

void DatatypeModel::fold(bool v)
{
    if (d_ptr->m_data.size() > 0)
    {
        auto root = d_ptr->m_data[0];
        for (auto &it : root->m_child)
        {
            nodeVisiable(it, v);
        }
    }
    m_title["fold"] = v;
    emit titleChanged();
}

void DatatypeModel::clearStatus() {
    for(int i=0;i<m_sortNodes.size();i++) {
        QStringList node = m_sortNodes.at(i).toStringList();
        for(int j=0;j<node.length();j++){
            QString originalStr=node[j];
            QRegularExpression regex("(,[^,]*),.*");
            QString resultStr = originalStr.replace(regex, "\\1," + QString("false"));
            node.replace(j, resultStr);
        }
        m_sortNodes.replace(i, node);
    }

    emit sortNodesChanged();
    if (d_ptr->m_data.size() > 0)
    {
        auto root = d_ptr->m_data[0];
        root->qmlSelected(false);
    }
}

void DatatypeModel::clearSelectBtn(int parentIndex, int index, QString inherName) {

    auto tags = inherName.split("-");
    if(tags.size()>0) {
        if(tags[0]==m_title["title"].toString()) {
            if(parentIndex!=-1) {
                setSelected(parentIndex,index,"false");
            }
            if (d_ptr->m_data.size() > 0)
            {
                auto root = d_ptr->m_data[0];
                root->qmlSelected(false);
            }
        }
    }

}

AllDatatypeModel::AllDatatypeModel(QObject *parent) : QObject(parent)
{
}

void AllDatatypeModel::praseData(boost::json::value&& v)
{
    for (auto it : m_allDatas)
    {
        auto ptr = it.value<DatatypeModel *>();
        ptr->deleteLater();
    }
    m_allDatas.clear();

    boost::system::error_code ec;
    auto obj = v.find_pointer("/items", ec);
    if (!ec && obj->if_array())
    {
        for (auto &it : obj->as_array())
        {
            DatatypeModel *node = new DatatypeModel(std::move(it));
            m_allDatas.append(QVariant::fromValue(node));
        }
    }
    emit allDatasChanged();
}

void AllDatatypeModel::clearStatus() {
    for(auto& it:m_allDatas) {
        it.value<DatatypeModel*>()->clearStatus();
    }
}

void AllDatatypeModel::clearSelectBtn(int parentIndex, int index, QString inherName) {
    for(auto& it:m_allDatas) {
        it.value<DatatypeModel*>()->clearSelectBtn(parentIndex,index,inherName);
    }
}

DatatypeModelManage::DatatypeModelManage(QObject *parent):QObject(parent) {
    try {
        auto tags = boost::json::parse(common::tags);
        auto tagsArry = tags.as_array();

        QHash<QString,AllDatatypeModel*> secTag;
        QHash<QString,AllSingleDatatypeModel*> fisTag;
        // 一级标签
        for(const auto& [key,value] : tagsArry[0].as_object()) {
            auto firs = new AllSingleDatatypeModel;
            boost::json::object tmp {{"items",value}};
            firs->praseData(std::move(tmp));
            fisTag.insert(key.data(),firs);
        }

        // 二级标签
        for(const auto& [key,value] : tagsArry[1].as_object()) {
            auto sec = new AllDatatypeModel;
            boost::json::object tmp {{"items",value}};
            sec->praseData(std::move(tmp));
            secTag.insert(key.data(),sec);
        }

        auto gt = boost::json::parse(common::grianType);
        auto arry = gt.as_array();
        for(auto obj : arry) {
            QString it;
            QString id = QString::number(obj.at("id").get_int64());
            it.append(id+"-");
            it.append(obj.at("name").get_string().c_str());
            m_graintypes.append(it);

            AllDatatypeModel *model1;
            AllSingleDatatypeModel *model2;
            if(fisTag.contains(id)) {
                model2 = fisTag[id];
            }else {
                model2 = new AllSingleDatatypeModel;
            }
            if(secTag.contains(id)){
                model1 = secTag[id];
            } else {
                model1 = new AllDatatypeModel;
            }
            m_tagModels[id] = { model1, model2 };
        }
        qDebug() << m_tagModels;
        emit graintypesChanged();

        // AllDatatypeModel* wheatAllDatatypeModel = new AllDatatypeModel;
        // wheatAllDatatypeModel->praseData(boost::json::parse(json_str));

        // AllSingleDatatypeModel* wheatAllSingleDatatypeModel = new AllSingleDatatypeModel;
        // wheatAllSingleDatatypeModel->praseData(boost::json::parse(json_str2));

        // AllDatatypeModel* cornAllDatatypeModel = new AllDatatypeModel;
        // cornAllDatatypeModel->praseData(boost::json::parse(json_str3));

        // AllSingleDatatypeModel* cornAllSingleDatatypeModel = new AllSingleDatatypeModel;
        // cornAllSingleDatatypeModel->praseData(boost::json::parse(json_str4));

        // m_tagModels["wheat"]= {wheatAllDatatypeModel, wheatAllSingleDatatypeModel};
        // m_tagModels["corn"]= {cornAllDatatypeModel, cornAllSingleDatatypeModel};
    } catch(...) { }
#ifdef TEST
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
                            },
                            {
                                "value": "呆白皱缩",
                                "items": []
                            },
                            {
                                "value": "开裂",
                                "items": []
                            },
                            {
                                "value": "黑色子囊壳",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "生芽",
                        "items": [
                            {
                                "value": "裂口",
                                "items": []
                            },
                            {
                                "value": "短芽",
                                "items": []
                            },
                            {
                                "value": "长芽",
                                "items": []
                            },
                            {
                                "value": "胡须",
                                "items": []
                            },
                            {
                                "value": "无麦胚",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "生霉",
                        "items": [
                            {
                                "value": "类似灰尘",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "虫蚀",
                        "items": [
                            {
                                "value": "啃咬",
                                "items": []
                            },
                            {
                                "value": "孔洞",
                                "items": []
                            }
                        ]
                    },
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
                            },
                            {
                                "value": "开裂",
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
                            },
                            {
                                "value": "烘干",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "正常",
                        "items": [
                            {
                                "value": "破皮",
                                "items": []
                            },
                            {
                                "value": "裂纹",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "杂质",
                        "items": [
                            {
                                "value": "有机",
                                "items": []
                            },
                            {
                                "value": "无机",
                                "items": []
                            },
                            {
                                "value": "无使用价值",
                                "items": []
                            },
                            {
                                "value": "虫",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "未熟粒",
                        "items": [
                            {
                                "value": "破皮",
                                "items": []
                            },
                            {
                                "value": "裂纹",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "带壳粒",
                        "items": [
                        ]
                    },
                    {
                        "value": "未知",
                        "items": [
                        ]
                    }
                ]
            }
        ]
    })";
    std::string json_str2 = R"({
        "graintype": "wheat",
        "items": [
            {
                "value": "缺陷程度",
                "items": [
                    {
                        "value": "重度",
                        "items": [
                        ]
                    },
                    {
                        "value": "中度",
                        "items": []
                    },
                    {
                        "value": "轻微",
                        "items": []
                    }
                ]
            },
            {
                "value": "缺陷区域",
                "items": [
                    {
                        "value": "胚乳",
                        "items": [
                        ]
                    },
                    {
                        "value": "胚部",
                        "items": []
                    },
                    {
                        "value": "尾部",
                        "items": []
                    },
                    {
                        "value": "整体",
                        "items": []
                    }
                ]
            },
            {
                "value": "缺陷形状",
                "items": [
                    {
                        "value": "片状",
                        "items": [
                        ]
                    },
                    {
                        "value": "斑块",
                        "items": []
                    },
                    {
                        "value": "点状",
                        "items": []
                    }
                ]
            },
            {
                "value": "颗粒姿态",
                "items": [
                    {
                        "value": "侧躺",
                        "items": [
                        ]
                    }
                ]
            },
            {
                "value": "缺陷颜色",
                "items": [
                    {
                        "value": "灰绿色",
                        "items": [
                        ]
                    },
                    {
                        "value": "白色",
                        "items": [
                        ]
                    },
                    {
                        "value": "黑色",
                        "items": [
                        ]
                    }
                ]
            },
            {
                "value": "干扰",
                "items": [
                    {
                        "value": "杂质",
                        "items": [
                        ]
                    },
                    {
                        "value": "有灰",
                        "items": [
                        ]
                    },
                    {
                        "value": "多颗粘连",
                        "items": [
                        ]
                    },
                    {
                        "value": "异色",
                        "items": [
                        ]
                    },
                    {
                        "value": "虫",
                        "items": [
                        ]
                    }
                ]
            }
        ]
    })";

    std::string json_str3 = R"({
        "graintype": "corn",
        "items": [
            {
                "value": "颗粒类型",
                "items": [
                    {
                        "value": "病斑",
                        "items": [
                            {
                                "value": "圆斑",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "生芽",
                        "items": [
                            {
                                "value": "裂口",
                                "items": []
                            },
                            {
                                "value": "短芽",
                                "items": []
                            },
                            {
                                "value": "长芽",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "生霉",
                        "items": [
                            {
                                "value": "粒面生霉",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "霉变",
                        "items": [
                            {
                                "value": "内霉",
                                "items": []
                            },
                            {
                                "value": "裂口",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "虫蚀",
                        "items": [
                            {
                                "value": "啃咬",
                                "items": []
                            },
                            {
                                "value": "孔洞",
                                "items": []
                            },
                            {
                                "value": "隧道",
                                "items": []
                            }
                        ]
                    },
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
                            },
                            {
                                "value": "五分之一",
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
                            },
                            {
                                "value": "烘干",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "正常",
                        "items": [
                            {
                                "value": "破皮",
                                "items": []
                            },
                            {
                                "value": "未知",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "杂质",
                        "items": [
                            {
                                "value": "有机",
                                "items": []
                            },
                            {
                                "value": "无机",
                                "items": []
                            },
                            {
                                "value": "无使用价值",
                                "items": []
                            },
                            {
                                "value": "虫",
                                "items": []
                            },
                            {
                                "value": "毒物",
                                "items": []
                            }
                        ]
                    },
                    {
                        "value": "无效",
                        "items": [
                        ]
                    }
                ]
            }
        ]
    })";


    std::string json_str4 = R"({
        "graintype": "wheat",
        "items": [
            {
                "value": "缺陷程度",
                "items": [
                    {
                        "value": "重度",
                        "items": [
                        ]
                    },
                    {
                        "value": "中度",
                        "items": []
                    },
                    {
                        "value": "轻微",
                        "items": []
                    }
                ]
            },
            {
                "value": "缺陷区域",
                "items": [
                    {
                        "value": "胚乳",
                        "items": [
                        ]
                    },
                    {
                        "value": "胚部",
                        "items": []
                    },
                    {
                        "value": "尾部",
                        "items": []
                    },
                    {
                        "value": "整体",
                        "items": []
                    }
                ]
            },
            {
                "value": "缺陷形状",
                "items": [
                    {
                        "value": "片状",
                        "items": [
                        ]
                    },
                    {
                        "value": "斑块",
                        "items": []
                    },
                    {
                        "value": "点状",
                        "items": []
                    },
                    {
                        "value": "裂口",
                        "items": []
                    }
                ]
            },
            {
                "value": "颗粒姿态",
                "items": [
                    {
                        "value": "侧躺",
                        "items": [
                        ]
                    }
                ]
            },
            {
                "value": "缺陷颜色",
                "items": [
                    {
                        "value": "灰绿色",
                        "items": [
                        ]
                    },
                    {
                        "value": "黄褐色",
                        "items": [
                        ]
                    },
                    {
                        "value": "白色",
                        "items": [
                        ]
                    },
                    {
                        "value": "黑色",
                        "items": [
                        ]
                    },
                    {
                        "value": "红色",
                        "items": [
                        ]
                    }
                ]
            },
            {
                "value": "干扰",
                "items": [
                    {
                        "value": "杂质",
                        "items": [
                        ]
                    },
                    {
                        "value": "有灰",
                        "items": [
                        ]
                    },
                    {
                        "value": "多颗粘连",
                        "items": [
                        ]
                    },
                    {
                        "value": "异色",
                        "items": [
                        ]
                    },
                    {
                        "value": "虫",
                        "items": [
                        ]
                    }
                ]
            }
        ]
    })";
#endif
}

QVariant DatatypeModelManage::getAllDataModel(QString type) {
    if(m_tagModels.contains(type)) {
        return QVariant::fromValue(m_tagModels[type].first);
    }
    return {};
}

QVariant DatatypeModelManage::getAllSingleDataModel(QString type) {
    if(m_tagModels.contains(type)) {
        return QVariant::fromValue(m_tagModels[type].second);
    }
    return {};
}

void DatatypeModelManage::reset() {
    for(auto& it:m_tagModels) {
        it.first->clearStatus();
        it.second->clearStatus();
    }

}

void DatatypeModelManage::clearSelectBtn(QString type, QString inherName, int parentIndex, int index)
{
    if(m_tagModels.contains(type)) {
        m_tagModels[type].first->clearSelectBtn(parentIndex,index,inherName);
        m_tagModels[type].second->clearSelectBtn(parentIndex,index,inherName);
    }
}
