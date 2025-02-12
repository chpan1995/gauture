#include <QApplication>
#include <QIcon>
#include <QDebug>
#include "Updater.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    if(argc<2) return -1;
    app.setWindowIcon(QIcon(":/resources/icon.png"));
    Updater dpdate(argv[1],argv[2]);
    dpdate.resize(836,520);
    dpdate.show();
    return app.exec();
}
