#ifndef LABELTAGS_H
#define LABELTAGS_H

#include <QObject>
#include <QQmlEngine>
#include <QAbstractListModel>

class LabelTagsItem;

class LabelTags : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QmlLabelTags)
public:
    enum TagTypes {
        SapTypeRole = Qt::UserRole+1,
        InherNameRole,
        FirstIndexRole,
        SecondIndexRole,
        TopNameRole,
        TraitRole
    };

    explicit LabelTags(QObject *parent = nullptr);
    Q_INVOKABLE void removeRow(QString sapType,QString inherName,int trait);
    Q_INVOKABLE void appendRow(QString sapType,QString inherName,int firstIndex,int secondIndex
                               ,QString topName,int trait);
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const  override;

private:
    QList<LabelTagsItem> m_datas;
};


class LabelTagsItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sapType MEMBER m_sapType NOTIFY sapTypeChanged FINAL)
    Q_PROPERTY(QString inherName MEMBER m_inherName NOTIFY inherNameChanged FINAL)
    Q_PROPERTY(int firstIndex MEMBER m_firstIndex NOTIFY firstIndexChanged FINAL)
    Q_PROPERTY(int secondIndex MEMBER m_secondIndex NOTIFY secondIndexChanged FINAL)
    Q_PROPERTY(QString topName MEMBER m_topName NOTIFY topNameChanged FINAL)
    Q_PROPERTY(int trait MEMBER m_trait NOTIFY traitChanged FINAL)
public:
    explicit LabelTagsItem(QObject *parent = nullptr);
    explicit LabelTagsItem(QString sapType,QString inherName,int firstIndex,int secondIndex
                           ,QString topName,int trait);
    LabelTagsItem(const LabelTagsItem& it);
    LabelTagsItem& operator=(const LabelTagsItem& it);
    LabelTagsItem& operator=(LabelTagsItem&& it);
signals:
    void sapTypeChanged();
    void inherNameChanged();
    void firstIndexChanged();
    void secondIndexChanged();
    void topNameChanged();
    void traitChanged();
private:
    QString m_sapType;
    QString m_inherName;
    int m_firstIndex;
    int m_secondIndex;
    QString m_topName;
    int m_trait;
};

#endif // LABELTAGS_H
