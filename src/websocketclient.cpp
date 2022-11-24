// Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include "websocketclient.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

//! [constructor]
WebSocketClient::WebSocketClient(const QUrl &url, bool debug, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_debug(debug)
{
    timer=new QTimer(this);
    m_wsServerFlag=false;
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::ondisconnected);
    connect(timer,&QTimer::timeout,this,&WebSocketClient::onTimeout);
    timer->start(10000);
    m_webSocket.open(QUrl(url));
}
//! [constructor]

//! [onConnected]
//连接服务器后，停止定时器，并且通知http服务器服务器上线
void WebSocketClient::onConnected()
{
    if (m_debug){
        qDebug() << "WebSocket connected";
        m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
        }
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
    timer->stop();
    m_wsServerFlag=true;
    emit serverStateChanged(m_wsServerFlag);
}
//! [onConnected]
//服务器断开连接后，重新启动定时器，每10秒检查服务器状态并尝试重连
void WebSocketClient::ondisconnected()
{
    if(m_debug){
        qDebug()<<"Connection lost, waiting for reconnect";
    }
    m_wsServerFlag=false;
    emit serverStateChanged(m_wsServerFlag);
    timer->start(10000);
}

//! [onTextMessageReceived]
//收到websocket服务器消息后通知http服务器
void WebSocketClient::onTextMessageReceived(QString message)
{
    if (m_debug){
        qDebug() << "WebSocket server message received:" << message;
    }
    //将收到的websocket信息发送给httpserver
    emit (receiveWsMsg(message));
//    m_webSocket.close();
}
//! [onTextMessageReceived]

//收到http请求消息后转发到服务器
void  WebSocketClient::onHttpRequestReceived(QString &msg){
    if (m_debug)
        qDebug() << "Message To be sent:" << msg;
     m_webSocket.sendTextMessage(msg);
};


void  WebSocketClient::onTimeout(){
    m_webSocket.open(m_url);
};
