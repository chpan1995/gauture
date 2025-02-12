#include "Login.h"

#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QApplication>
#include <QProcess>
#include <QMessageBox>

#include "Utils.h"
#include "log.h"

std::optional<boost::json::value> praseRespose(const char *response, std::size_t lenth)
{
    boost::json::value v;
    boost::json::stream_parser p;
    boost::system::error_code ec;
    p.write(response, lenth, ec);
    if (ec) {
        logging::log_info(RL, "{}--{}", "parse json failed", response);
        return std::nullopt;
    } else {
        v = p.release();
        auto err = v.find_pointer("/eMessage", ec);
        if (!ec) {
            // 请求错误
            logging::log_info(RL, "请求错误:{}{}", "parse json failed", err->get_string().c_str());
            return std::nullopt;
        }
    }
    return v;
}

Login::Login()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    initLayout();
    setWindowIcon(QIcon(":images/icon.png"));

    m_HttpClient = std::make_shared<HttpClient>(m_ioc);
    m_thd = std::thread([this] {
        auto work(boost::asio::make_work_guard(m_ioc));
        m_ioc.run();
    });

    QFile file(QApplication::applicationDirPath()+"/inf");
    if(file.open(QIODevice::ReadWrite)){
        QDataStream in(&file);
        bool checked=false;
        QString username,password;
        in >> checked >> username >> password;
        if(checked) {
            m_username->setText(username);
            m_password->setText(password);
            m_checkBox->setChecked(checked);
        }
        file.close();
    }else {
        logging::log_error(RL,"open file failed");
    }
    m_HttpClient->addRequest(HttpRequest("192.168.1.158",
                                         "8080",
                                         "/api/update",
                                         [this](const char *response, std::size_t lenth) {
                                             boost::system::error_code ec;
                                             auto v = praseRespose(response, lenth);
                                             if (v.has_value() && v->is_object()) {
                                                 auto obj = v->as_object();
                                                 if(obj.contains("linux")){
                                                     auto it = obj.at("linux").as_object();
                                                     QString version = it.at("version").as_string().c_str();
                                                     QStringList f= version.split("_");
                                                     QString s = f[1].remove(".zip");
                                                     int v = s.toInt();
                                                     logging::log_info(RL,"server version: {}" , version.toStdString());
#ifdef Q_OS_UNIX
                                                     if(v > LINUX_VERSION) {
                                                         QMetaObject::invokeMethod(this,[version]{
                                                             QMessageBox::information(nullptr,"提示","检测到新版本请升级");
                                                             QProcess::startDetached(QApplication::applicationDirPath()+"/../Gzupdate.sh"
                                                                                     ,{version,"http://192.168.1.158:8080/"});
                                                             exit(0);
                                                         },Qt::QueuedConnection);
                                                     }
#elif Q_OS_WIN
                                                     if(obj.contains("win")){
                                                         auto it = obj.at("win").as_object();
                                                         QString version = it.at("version").as_string().c_str();
                                                         QStringList f= version.split("_");
                                                         QString s = f[1].remove(".zip");
                                                         int v = s.remove(".zip").toInt();
                                                         if(v > WINDOWS_VERSION) {
                                                             QMetaObject::invokeMethod(this,[version]{
                                                                 QMessageBox::information(nullptr,"提示","检测到新版本请升级");
                                                                 QProcess::startDetached(QApplication::applicationDirPath()+"/../GPTupdate.exe"
                                                                                         ,{version,"http://192.168.1.158:8080/"});
                                                                 exit(0);
                                                             },Qt::QueuedConnection);
                                                         }
                                                     }
#endif
                                                 }
                                             }else {
                                                 logging::log_error(RL,"/api/update error");
                                             }
                                         }));
}

Login::~Login()
{
    m_ioc.stop();
    if (m_thd.joinable())
        m_thd.join();
}

void Login::slotBtnLogin()
{
    if(m_username->text()==""){
        m_animationUserName->stop();
        m_animationUserName->start();
        return;
    }
    if(m_password->text()==""){
        m_tipPssword->setText("密码不能为空");
        m_animationUserName->stop();
        m_animationPassword->start();
        return;
    }
    m_HttpClient->addRequest(HttpRequest("192.168.1.158",
                                         "8080",
                                         "/api/user/login",
                                         boost::json::serialize(boost::json::object(
                                             {{"username", m_username->text().toStdString()},
                                              {"password", m_password->text().toStdString()}})),
                                         [this](const char *response, std::size_t lenth) {
                                             boost::system::error_code ec;
                                             auto v = praseRespose(response, lenth);
                                             if (v.has_value()) {
                                                 if (v->as_object().contains("code")
                                                     &&v->as_object().at("code").as_int64() == 200)
                                                 {
                                                     common::userid=v->as_object().at("userid").as_int64();
                                                     emit sigLoginStatus(true);
                                                 }
                                                 if(!v->as_object().contains("code")) {
                                                     emit sigServerFailed();
                                                 }
                                                 emit sigLoginStatus(false);
                                             }else {
                                                 emit sigServerFailed();
                                             }
                                         }));
}

void Login::slotOnLoginStatus(bool f) {
    if (f) {
        if(m_checkBox->checkState()==Qt::Checked) {
            QFile file(QApplication::applicationDirPath()+"/inf");
            if(file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
                QDataStream out(&file);
                out << true << m_username->text() << m_password->text();
                file.close();
            }else {
                logging::log_error(RL,"open file failed");
            }
        }
        m_loginUserName = m_username->text();
        common::username = m_username->text().toStdString();
        accept();
    }else {
        m_tipPssword->setText("用户名或密码错误");
        m_animationUserName->stop();
        m_animationPassword->start();
    }
}

void Login::slotOnServerFailed() {
    m_tipPssword->setText("请求错误");
    m_animationUserName->stop();
    m_animationPassword->start();
}

void Login::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    BackWidget *mainWidget = new BackWidget(this);
    mainWidget->setObjectName("mainWidget");
    mainLayout->addWidget(mainWidget);
    QVBoxLayout *vLayout = new QVBoxLayout(mainWidget);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(0, 0, 4, 0);
    topLayout->setSpacing(2);
    QPushButton *btnMin = new QPushButton(this);
    btnMin->setObjectName("btnMin");

    QPushButton *btnClose = new QPushButton(this);
    btnClose->setObjectName("btnClose");
    btnClose->setFlat(true);
    topLayout->addStretch();
    topLayout->addWidget(btnMin);
    topLayout->addWidget(btnClose);
    vLayout->addLayout(topLayout);

    QHBoxLayout *logLayout = new QHBoxLayout;
    QWidget *logw = new QWidget(this);
    logw->setObjectName("logw");
    logLayout->addStretch();
    logLayout->addWidget(logw);
    logLayout->addStretch();
    vLayout->addSpacing(72);
    vLayout->addLayout(logLayout);

    QVBoxLayout *contenLayout = new QVBoxLayout;
    contenLayout->setSpacing(0);
    contenLayout->setContentsMargins(32, 0, 32, 0);
    QLabel* tipuser = new QLabel("账户不能为空",this);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(tipuser);
    tipuser->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(0);
    m_animationUserName = new QPropertyAnimation(opacityEffect,"opacity");
    m_animationUserName->setStartValue(QVariant(1));
    m_animationUserName->setEndValue(QVariant(0));
    m_animationUserName->setDuration(2000);
    m_animationUserName->setEasingCurve(QEasingCurve::InQuad);

    m_tipPssword = new QLabel("",this);
    QGraphicsOpacityEffect *opacityEffect_pswd = new QGraphicsOpacityEffect(m_tipPssword);
    m_tipPssword->setGraphicsEffect(opacityEffect_pswd);
    opacityEffect_pswd->setOpacity(0);

    m_animationPassword = new QPropertyAnimation(opacityEffect_pswd,"opacity");
    m_animationPassword->setStartValue(QVariant(1));
    m_animationPassword->setEndValue(QVariant(0));
    m_animationPassword->setDuration(2000);
    m_animationPassword->setEasingCurve(QEasingCurve::InQuad);

    m_username = new QLineEdit(this);
    m_password = new QLineEdit(this);
    m_password->setEchoMode(QLineEdit::Password);
    m_btnLogin = new QPushButton("登录", this);
    m_username->setPlaceholderText("输入用户名");
    m_password->setPlaceholderText("输入密码");
    m_btnLogin->setObjectName("btnLogin");

    contenLayout->addWidget(m_username);
    contenLayout->addWidget(tipuser);
    contenLayout->addWidget(m_password);
    contenLayout->addWidget(m_tipPssword);
    contenLayout->addWidget(m_btnLogin);
    contenLayout->addSpacing(20);
    vLayout->addSpacing(56);
    vLayout->addLayout(contenLayout);

    QHBoxLayout *btmLayout = new QHBoxLayout;
    btmLayout->setContentsMargins(32, 0, 32, 0);
    m_checkBox = new QCheckBox("记住密码", this);
    btmLayout->addWidget(m_checkBox);
    btmLayout->addStretch();
    vLayout->addSpacing(16);
    vLayout->addLayout(btmLayout);

    vLayout->addStretch();
    readStylesheet();
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 90));
    shadowEffect->setOffset(0, 0); // 添加这行使阴影均匀分布
    mainWidget->setGraphicsEffect(shadowEffect);

    connect(btnMin, &QPushButton::clicked, this, [this] { showMinimized(); });
    connect(btnClose, &QPushButton::clicked, this, [this] { reject(); });
    connect(m_btnLogin, &QPushButton::clicked, this, &Login::slotBtnLogin);
    connect(this, &Login::sigLoginStatus, this, &Login::slotOnLoginStatus);
    connect(this, &Login::sigServerFailed, this, &Login::slotOnServerFailed);
}

void Login::readStylesheet()
{
    QFile file(":/qss/login.qss");
    if (file.exists()) {
        file.open(QFile::ReadOnly);
        QString styleSheet;
        styleSheet.append(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}

BackWidget::BackWidget(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
}

void BackWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, 182, 182, QPixmap(":/images/logintp.png"));
    p.drawPixmap(width() - 154, height() - 154, 154, 154, QPixmap(":/images/loginbtm.png"));
    return QWidget::paintEvent(event);
}
