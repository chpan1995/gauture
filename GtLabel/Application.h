#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <boost/function.hpp>
#include <QProcess>

#include "WebscoketClient.h"

class ReviceWebSocketdata : public ObsWebContent {
public:
    explicit ReviceWebSocketdata(WebscoketSignature v):m_signature(v) { }
protected:
    void praseData(boost::json::value& vaule) override {
        m_signature(vaule);
    }
private:
    WebscoketSignature m_signature;
};

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int& argc, char **argv);
    void initWebScoket(std::string name);
signals:
    void sigRcvlogout();
private:
    void slotRcvlogout();
private:
    void praseData(boost::json::value& vaule);
private:
    std::shared_ptr<WebscoketClient> m_webSocket;
    std::shared_ptr<ReviceWebSocketdata> m_reviceWebSocketdata;
    QProcess m_process;
};

#endif // APPLICATION_H
