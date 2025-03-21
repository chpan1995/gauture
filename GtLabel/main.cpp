﻿#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QTimer>

#include "Application.h"
#include "UI/FrameLessView.h"

#include "UI/Login.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    Login login;
    if(login.exec() != QDialog::Accepted){
        return 0;
    }
    app.initWebScoket(login.username().toStdString(),common::server2IP,common::server2Port);
    // 设置 QML 控件样式为 Basic
    QQuickStyle::setStyle("Basic");
    FrameLessView *view = new FrameLessView;
    // view->engine()->addImportPath(":/");
    view->engine()->addImportPath(":/qt/qml/");
    view->rootContext()->setContextProperty("view", view);
    view->rootContext()->setContextProperty("ctx_username", QString::fromStdString(common::nickname));
    view->loadFromModule("ui_main", "Main");
    view->setWidth(1294);
    view->setHeight(800);
    view->showNormal();
    QTimer::singleShot(0,[&]{view->showMaximized();});
    QObject::connect(view->engine(), &QQmlEngine::quit, qApp, &QCoreApplication::quit);
    app.setWindowIcon(QIcon(":images/icon.png"));
    return app.exec();
}
