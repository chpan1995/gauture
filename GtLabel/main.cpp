#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "UI/FrameLessView.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    FrameLessView *view = new FrameLessView;
    view->engine()->addImportPath(":/");
    view->rootContext()->setContextProperty("view", view);
    view->loadFromModule("ui_main","Main");
    view->setWidth(1294);
    view->setHeight(800);
    view->showNormal();
    QObject::connect(view->engine(), &QQmlEngine::quit, qApp, &QCoreApplication::quit);
    return app.exec();
}
