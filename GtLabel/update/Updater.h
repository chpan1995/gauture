#ifndef UPDATER_H
#define UPDATER_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class Updater : public QWidget
{
    Q_OBJECT
public:
    explicit Updater(QString version,QString url,QWidget *parent = nullptr);
    void initLayout();
    void readStylesheet();

    static QRect calcCenterGeo(const QRect &screenGeo, const QSize &normalSize);
private slots:
    void replyFinished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFile();
signals:
private:
    QProgressBar* m_progressBar;
    QNetworkAccessManager* m_networkAccessManager;
    QNetworkReply* m_res {nullptr};
    QString m_url;
    QFile* m_file;
    QString m_version;
};

#endif // UPDATER_H
