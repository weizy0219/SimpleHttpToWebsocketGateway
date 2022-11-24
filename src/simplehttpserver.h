#ifndef SIMPLEHTTPSERVER_H
#define SIMPLEHTTPSERVER_H
#include<QHttpServer>
#include "qjsondocument.h"
#include "qjsonobject.h"

class SimpleHttpServer : public QObject
{
    Q_OBJECT
public:
    explicit SimpleHttpServer(const QHostAddress host, const int port,bool debug = false, QObject *parent = nullptr);
public Q_SLOTS:
     void onReceiveWsResponse(QString &msg);
     void onWsServerStateChanged(bool &flag);

Q_SIGNALS:
    void testId(int &newId);
    void receivePostMsg(QString &msg);

private Q_SLOTS:
    void onTestId(int &newId);

//    void onTextMessageReceived(QString message);

private:
    QHttpServer m_simpleHttpServer;
    QHostAddress m_host;
    int m_port;
    bool m_debug;
    bool m_wsServerState;
    void initRoutes(int port);
    const QJsonObject byteArrayToJsonObject(const QByteArray &arr);
    QList<QString> responseList;

};

#endif // SIMPLEHTTPSERVER_H
