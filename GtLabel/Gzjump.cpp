#include <QCoreApplication>
#include <QProcess>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QProcess p;
    p.startDetached(QCoreApplication::applicationDirPath()+"/appGtLabel");
    QTimer::singleShot(100,[&]{app.exit(0);});
    return app.exec();
}
