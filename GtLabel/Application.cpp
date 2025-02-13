#include "Application.h"
#include "Utils.h"

#include <QProcess>

Application::Application(int& argc, char **argv) : QApplication(argc,argv)
{
    m_reviceWebSocketdata=std::make_shared<ReviceWebSocketdata>(
        boost::bind(&Application::praseData,this,boost::placeholders::_1));
    QObject::connect(this,&Application::sigRcvlogout,this,&Application::slotRcvlogout);
}

void Application::initWebScoket(std::string name) {
    m_webSocket=std::make_shared<WebscoketClient>(name);
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
