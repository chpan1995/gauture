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
public:
    explicit DataNode(DataNode* parent=nullptr);

    QString tagName();
    void setTagName(QString v);

    int deep();
    void setDeep(int v);

    QVariantList inheritsName();
    void setInheritsName(QVariantList v);

    void setInheritsName();

signals:
    void tagNameChanged();
    void deepChanged();
    void inheritsNameChanged();
public:
    DataNode* m_parent;
private:
    QString m_tagName;
    int m_deep;
    QVariantList m_inheritsName;
};

class DatatypeModelPrivate;
class DatatypeModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<DataNode> treeNodes READ treeNodes NOTIFY treeNodesChanged FINAL)
public:
    explicit DatatypeModel(boost::json::value&& data,QObject *parent = nullptr);
    ~DatatypeModel();

    QQmlListProperty<DataNode> treeNodes();

    // test
    void updateData(boost::json::value& v);

signals:
    void treeNodesChanged();
public:
    DatatypeModelPrivate* d_ptr;
    std::uint8_t m_index {0};
    // Q_DECLARE_PRIVATE(DatatypeModel)
};

Q_DECLARE_METATYPE(DatatypeModel)

class AllDatatypeModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariantList allDatas MEMBER m_allDatas NOTIFY allDatasChanged FINAL)
public:
    explicit AllDatatypeModel(QObject* parent=nullptr);
    void praseData(boost::json::value&& v);
signals:
    void allDatasChanged();
private:
    QVariantList m_allDatas;
};

#endif // DATATYPEMODEL_H
