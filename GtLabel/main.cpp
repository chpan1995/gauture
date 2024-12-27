#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "UI/FrameLessView.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    FrameLessView *view = new FrameLessView;
    view->engine()->addImportPath(":/");
    view->setSource(QUrl("qrc:/ui_main/Main.qml"));

    view->showNormal();
    return app.exec();
}
