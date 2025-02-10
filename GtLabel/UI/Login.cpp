#include "Login.h"

#include <QFile>
#include <QPainter>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>

Login::Login() {
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground,true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    initLayout();
    setWindowIcon(QIcon(":images/icon.png"));
}

void Login::initLayout() {
    QHBoxLayout* mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    BackWidget* mainWidget=new BackWidget(this);
    mainWidget->setObjectName("mainWidget");
    mainLayout->addWidget(mainWidget);
    QVBoxLayout* vLayout=new QVBoxLayout(mainWidget);
    vLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout* topLayout=new QHBoxLayout;
    topLayout->setContentsMargins(0,0,4,0);
    topLayout->setSpacing(2);
    QPushButton* btnMin=new QPushButton(this);
    btnMin->setObjectName("btnMin");

    QPushButton* btnClose=new QPushButton(this);
    btnClose->setObjectName("btnClose");
    btnClose->setFlat(true);
    topLayout->addStretch();
    topLayout->addWidget(btnMin);
    topLayout->addWidget(btnClose);
    vLayout->addLayout(topLayout);

    QHBoxLayout* logLayout=new QHBoxLayout;
    QWidget* logw=new QWidget(this);
    logw->setObjectName("logw");
    logLayout->addStretch();
    logLayout->addWidget(logw);
    logLayout->addStretch();
    vLayout->addSpacing(72);
    vLayout->addLayout(logLayout);

    QVBoxLayout* contenLayout=new QVBoxLayout;
    contenLayout->setSpacing(16);
    contenLayout->setContentsMargins(32,0,32,0);
    m_username = new QLineEdit(this);
    m_password = new QLineEdit(this);
    m_password->setEchoMode(QLineEdit::Password);
    m_btnLogin = new QPushButton("登录",this);
    m_username->setPlaceholderText("输入用户名");
    m_password->setPlaceholderText("输入密码");
    m_btnLogin->setObjectName("btnLogin");
    contenLayout->addWidget(m_username);
    contenLayout->addWidget(m_password);
    contenLayout->addWidget(m_btnLogin);
    vLayout->addSpacing(56);
    vLayout->addLayout(contenLayout);

    QHBoxLayout* btmLayout = new QHBoxLayout;
    btmLayout->setContentsMargins(32,0,32,0);
    m_checkBox = new QCheckBox("记住密码",this);
    btmLayout->addWidget(m_checkBox);
    btmLayout->addStretch();
    vLayout->addSpacing(16);
    vLayout->addLayout(btmLayout);

    vLayout->addStretch();
    readStylesheet();
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 90));
    shadowEffect->setOffset(0, 0);  // 添加这行使阴影均匀分布
    mainWidget->setGraphicsEffect(shadowEffect);

    connect(btnMin,&QPushButton::clicked,this,[this]{showMinimized();});
    connect(btnClose,&QPushButton::clicked,this,[this]{reject();});
}

void Login::readStylesheet() {
    QFile file(":/qss/login.qss");
    if (file.exists()) {
        file.open(QFile::ReadOnly);
        QString styleSheet;
        styleSheet.append(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}

BackWidget::BackWidget(QWidget *parent):QWidget{parent} {
    setAttribute(Qt::WA_StyledBackground,true);
}

void BackWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0,0,182,182,QPixmap(":/images/logintp.png"));
    p.drawPixmap(width()-154,height()-154,154,154,QPixmap(":/images/loginbtm.png"));
    return QWidget::paintEvent(event);
}

