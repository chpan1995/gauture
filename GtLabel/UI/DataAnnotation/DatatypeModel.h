#ifndef DATATYPEMODEL_H
#define DATATYPEMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <boost/json.hpp>

class DataNode : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(QString tagName READ tagName WRITE setTagName NOTIFY tagNameChanged FINAL)
    Q_PROPERTY(int deep READ deep WRITE setDeep NOTIFY deepChanged FINAL)
    Q_PROPERTY(QVariantList inheritsName READ inheritsName WRITE setInheritsName NOTIFY inheritsNameChanged FINAL)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged FINAL)
    Q_PROPERTY(bool visiable READ visiable WRITE setVisiable NOTIFY visiableChanged FINAL)
public:
    explicit DataNode(DataNode* parent=nullptr);

    QString tagName();
    void setTagName(QString v);

    int deep();
    void setDeep(int v);

    QVariantList inheritsName();
    void setInheritsName(QVariantList v);

    void setInheritsName();

    int getDeep();

    bool selected();
    void setSelected(bool v);

    bool visiable();
    void setVisiable(bool v);

    inline void addChild(DataNode* node) { m_child.append(node); }

    inline DataNode* child(std::uint32_t index) {
        if(index>=m_child.size()) return nullptr;
        return m_child[index];
    }
    Q_INVOKABLE void qmlSelected(bool v);

signals:
    void tagNameChanged();
    void deepChanged();
    void inheritsNameChanged();
    void selectedChanged();
    void visiableChanged();
public:
    DataNode* m_parent;
    QList<DataNode*> m_child;
private:
    QString m_tagName;
    int m_deep;
    QVariantList m_inheritsName;
    bool m_selected {false};
    bool m_visiable {true};
};

class DatatypeModelPrivate;
class DatatypeModel : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(QQmlListProperty<DataNode> treeNodes READ treeNodes NOTIFY treeNodesChanged FINAL)
    Q_PROPERTY(QVariantList sortNodes MEMBER m_sortNodes NOTIFY sortNodesChanged FINAL)
    Q_PROPERTY(QVariantMap title MEMBER m_title NOTIFY titleChanged FINAL)

public:
    explicit DatatypeModel(boost::json::value&& data,QObject *parent = nullptr);
    ~DatatypeModel();

    QQmlListProperty<DataNode> treeNodes();

    void updateData(boost::json::value& v);

    // 多级菜单设置选中状态
    Q_INVOKABLE void setSelected(int parentIndex,int index, QString v);

    Q_INVOKABLE void fold(bool v);

    void clearStatus();

    void clearSelectBtn(int parentIndex,int index,QString inherName);
signals:
    void treeNodesChanged();
    void sortNodesChanged();
    void titleChanged();
public:
    DatatypeModelPrivate* d_ptr;
    std::uint8_t m_index {0};
    QVariantList m_sortNodes {};
    QVariantMap m_title;
    // Q_DECLARE_PRIVATE(DatatypeModel)
};

Q_DECLARE_METATYPE(DatatypeModel)

class AllDatatypeModel : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AnnotationIndexModel)

    Q_PROPERTY(QVariantList allDatas MEMBER m_allDatas NOTIFY allDatasChanged FINAL)
public:
    explicit AllDatatypeModel(QObject* parent=nullptr);
    void praseData(boost::json::value&& v);
    void clearStatus();
    void clearSelectBtn(int parentIndex,int index,QString inherName);
signals:
    void allDatasChanged();
private:
    QVariantList m_allDatas;
};
Q_DECLARE_METATYPE(AllDatatypeModel)

class AllSingleDatatypeModel:public AllDatatypeModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AnnotationSingleIndexModel)
public:
    // explicit AllSingleDatatypeModel(QObject* parent=nullptr);
};
Q_DECLARE_METATYPE(AllSingleDatatypeModel)

class DatatypeModelManage:public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QmlDatatypeModelManage)
public:
    explicit DatatypeModelManage(QObject* parent=nullptr);
    Q_INVOKABLE QVariant getAllDataModel(QString type);
    Q_INVOKABLE QVariant getAllSingleDataModel(QString type);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void clearSelectBtn(QString type,QString inherName,int parentIndex,int index);
private:
    QHash<QString,std::pair<AllDatatypeModel*,AllSingleDatatypeModel*>> m_tagModels;
};

#endif // DATATYPEMODEL_H
