#include "Application.h"

#include <QSettings>
#include <QProcess>

Application::Application(int& argc, char **argv) : QApplication(argc,argv)
{
    m_reviceWebSocketdata=std::make_shared<ReviceWebSocketdata>(
        boost::bind(&Application::praseData,this,boost::placeholders::_1));
    QObject::connect(this,&Application::sigRcvlogout,this,&Application::slotRcvlogout);

    QSettings settings(QApplication::applicationDirPath()+"/config.ini",QSettings::IniFormat);
    // settings.setValue("serveralgo/ip","192.168.1.108");
    // settings.setValue("serveralgo/port","8083");

    // settings.setValue("server1/ip","192.168.1.158");
    // settings.setValue("server1/port","8080");

    // settings.setValue("server2/ip","192.168.1.158");
    // settings.setValue("server2/port","8081");
    common::serveralgoIP=settings.value("serveralgo/ip","192.168.1.108").toString().toStdString();
    common::serveralgoPort=settings.value("serveralgo/port","8083").toString().toStdString();

    common::server1IP=settings.value("server1/ip","192.168.1.158").toString().toStdString();
    common::server1Port=settings.value("server1/port","8080").toString().toStdString();

    common::server2IP=settings.value("server2/ip","192.168.1.158").toString().toStdString();
    common::server2Port=settings.value("server2/port","8081").toString().toStdString();
}

void Application::initWebScoket(std::string name,std::string ip,std::string port) {
    m_webSocket=std::make_shared<WebscoketClient>(name,ip,port);
    m_webSocket->attach(m_reviceWebSocketdata);
}

void Application::slotRcvlogout() {
    m_process.startDetached(QCoreApplication::applicationDirPath()+"/Gzjump");
#ifdef _WIN32
    ExitProcess(0);
#endif
    exit(0);
}

void Application::praseData(boost::json::value &v) {
    boost::system::error_code ec;
    auto cmd = v.find_pointer("/commond", ec);
    if (!ec) {
        if(cmd->get_string() == "logout") {
            emit sigRcvlogout();
        }
        else if(cmd->get_string() == "ServerDown") {
            qDebug() << "服务器故障";
            postEvent(this,new NetState(false));
        }
        else if(cmd->get_string() == "ServerUP") {
            postEvent(this,new NetState(true));
        }
    }
}
