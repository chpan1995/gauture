#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

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
private:
    void initLayout();
    void readStylesheet();
private:
    QLineEdit* m_username;
    QLineEdit* m_password;
    QPushButton* m_btnLogin;
    QCheckBox* m_checkBox;
};

#endif // LOGIN_H
