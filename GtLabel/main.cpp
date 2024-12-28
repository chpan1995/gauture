#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "UI/FrameLessView.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    FrameLessView *view = new FrameLessView;
    view->engine()->addImportPath(":/");
    view->loadFromModule("ui_main","Main");
    view->showNormal();
    return app.exec();
}
