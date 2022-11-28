// Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtHttpServer>
#include "websocketclient.h"
#include "simplehttpserver.h"
#include "config.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //使用QCmmandLineParser解析命令行参数，支持 -d 来进入debug模式
    QCommandLineParser parser;
    parser.setApplicationDescription("http websocket网关，http://www.wesinx.com");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);

    //读取配置文件
    Config config(a.applicationDirPath(),debug);

    //启动WebSocket客户端
    QString wsUrl=config.findConf("wsserveraddr","ws://127.0.0.1:3390").toString();
    WebSocketClient client(QUrl(wsUrl), debug);
    int port=18000;

    //启动http服务器
    SimpleHttpServer httpServer(QHostAddress::LocalHost,config.findConf("httpserverip",18000).toInt(),debug);
    //config.setConf("test",10);
    //config.delConf("test");

    //建立信号和槽连接
    //当http服务器收到post请求后，转发请求到websocket服务器
    QObject::connect(&httpServer,&SimpleHttpServer::receivePostMsg,&client,&WebSocketClient::onHttpRequestReceived);
    //当websocket客户端收到服务器消息后，存放到http服务器数组中，等待用户通过get请求获取
    QObject::connect(&client,&WebSocketClient::receiveWsMsg,&httpServer,&SimpleHttpServer::onReceiveWsResponse);
    //websocket服务器离线或在线后，通知用户
    QObject::connect(&client,&WebSocketClient::serverStateChanged,&httpServer,&SimpleHttpServer::onWsServerStateChanged);
    QObject::connect(&httpServer,&SimpleHttpServer::receiveExitCmd,&a,QCoreApplication::quit);

    return a.exec();
}


