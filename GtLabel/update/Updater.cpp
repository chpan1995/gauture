#include "Updater.h"

#include <QApplication>
#include <QProcess>
#include <QScreen>
#include "quazip/JlCompress.h"

//
bool copyDir(const QString &source, const QString &destination, bool override)
{
    QDir d;
    d.mkdir(destination);
    QDir directory(source);
    if (!directory.exists())
    {
        return false;
    }

    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();

    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    //for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }

    return !error;
}

Updater::Updater(QString version,QString url,QWidget *parent)
    : QWidget{parent},m_version(version),m_url(url),
    m_networkAccessManager(new QNetworkAccessManager(this))
{
    setWindowFlags(Qt::FramelessWindowHint);
    setObjectName("Updater");
    initLayout();
    readStylesheet();
    m_url =m_url+version;
    QNetworkRequest request;
    request.setUrl(m_url);
    m_res= m_networkAccessManager->get(request);
    if(m_res){
        connect(m_res,&QNetworkReply::finished, this, &Updater::replyFinished);
        connect(m_res,&QNetworkReply::downloadProgress,this,&Updater::downloadProgress);
        connect(m_res,&QNetworkReply::readyRead,this,&Updater::downloadFile);
    }
    m_file=new QFile(this);
    m_file->setFileName(QApplication::applicationDirPath()+"/"+version);
    m_file->open(QFile::ReadWrite);
}

void Updater::initLayout()
{
    QHBoxLayout* layout=new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    QWidget* mainw=new QWidget;
    mainw->setObjectName("mainw");
    layout->addWidget(mainw);
    QVBoxLayout* mainLayout=new QVBoxLayout(mainw);
    mainLayout->addStretch();

    QHBoxLayout* hlayout=new QHBoxLayout;
    mainLayout->addLayout(hlayout);
    m_progressBar =new QProgressBar;
    m_progressBar->setMaximumHeight(6);
    m_progressBar->setTextVisible(false);
    hlayout->addWidget(m_progressBar);

    auto geo = calcCenterGeo(screen()->availableGeometry(), size());
    if (minimumWidth() > geo.width() || minimumHeight() > geo.height()) {
        setMinimumSize(geo.size());
    }
    setGeometry(geo);
    update();
}

void Updater::readStylesheet()
{
    QFile file(":/resources/update.qss");
    if (file.exists()) {
        file.open(QFile::ReadOnly);
        QString styleSheet;
        styleSheet.append(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
}

QRect Updater::calcCenterGeo(const QRect &screenGeo, const QSize &normalSize)
{
    int w = normalSize.width();
    int h = normalSize.height();
    int x = screenGeo.x() + (screenGeo.width() - w) / 2;
    int y = screenGeo.y() + (screenGeo.height() - h) / 2;
    if (screenGeo.width() < w) {
        x = screenGeo.x();
        w = screenGeo.width();
    }
    if (screenGeo.height() < h) {
        y = screenGeo.y();
        h = screenGeo.height();
    }

    return { x, y, w, h };
}

void Updater::replyFinished()
{
    if(m_res){
        m_res->abort();
        m_res->deleteLater();
        m_file->flush();
        m_file->close();
#ifdef linux
        JlCompress::extractDir(QApplication::applicationDirPath()+"/"+m_version,QDir::homePath());
        QString cmd;
        cmd="rm -rf /opt/appGtLabel/*";
        system(cmd.toStdString().c_str());
        cmd="cp -rf "+QDir::homePath()+"/appGtLabel/* /opt/appGtLabel/";
        system(cmd.toStdString().c_str());
        QProcess::startDetached("/opt/appGtLabel/appGtLabel.sh",{});
        cmd="rm -rf "+QDir::homePath()+"/appGtLabel";
        system(cmd.toStdString().c_str());
#elif _WIN32
        QProcess p(0);
        p.start("cmd",QStringList() << "/c" <<"taskkill /f /t /im appGtLabel.exe");
        p.waitForFinished();
        JlCompress::extractDir(QApplication::applicationDirPath()+"/"+m_version,"C:/pack");
        QDir dir_back(QApplication::applicationDirPath());
        dir_back.removeRecursively();

        copyDir("C:/pack",QApplication::applicationDirPath(),true);
        QProcess::startDetached(QApplication::applicationDirPath()+"/appGtLabel.exe",QStringList());
        QDir dir("C:/pack");
        dir.removeRecursively();
        QFile f;
        f.remove(QApplication::applicationDirPath()+"/"+m_version);
#endif
        exit(0);
    }
}

void Updater::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_progressBar->setMaximum(bytesTotal);
    m_progressBar->setValue(bytesReceived);
}

void Updater::downloadFile()
{
    if(m_res) m_file->write(m_res->readAll());
}
