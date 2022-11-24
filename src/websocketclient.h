#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

// Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QTimer>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QUrl &url, bool debug = false, QObject *parent = nullptr);

public Q_SLOTS:
     void onHttpRequestReceived(QString &msg);
Q_SIGNALS:
    void receiveWsMsg(QString &msg);
    void serverStateChanged(bool &flag);

private Q_SLOTS:
    void onConnected();
    void onTimeout();
    void onTextMessageReceived(QString message);
    void ondisconnected();


private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
    bool m_wsServerFlag;
    QTimer *timer;
};

#endif // ECHOCLIENT_H
