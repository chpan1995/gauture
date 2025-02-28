#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <thread>
#include <QLabel>
#include <QPropertyAnimation>

#include "HttpClient.h"

#define LINUX_VERSION 1004
#define WINDOWS_VERSION 1005

class BackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BackWidget(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class Login : public QDialog
{
    Q_OBJECT
public:
    Login();
    ~Login();
    inline QString username() {
        return m_loginUserName;
    }
signals:
    void sigLoginStatus(bool f);
    void sigServerFailed();
private slots:
    void slotBtnLogin();
    void slotOnLoginStatus(bool f);
    void slotOnServerFailed();
private:
    void initLayout();
    void readStylesheet();
private:
    QLineEdit* m_username;
    QLineEdit* m_password;
    QPushButton* m_btnLogin;
    QCheckBox* m_checkBox;
    QPropertyAnimation* m_animationUserName;
    QPropertyAnimation* m_animationPassword;
    QLabel* m_tipPssword;
    std::shared_ptr<HttpClient> m_HttpClient;
    boost::asio::io_context m_ioc;
    std::thread m_thd;
    QString m_loginUserName;
};

#endif // LOGIN_H
