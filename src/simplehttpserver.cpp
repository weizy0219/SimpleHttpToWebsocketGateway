#include "simplehttpserver.h"

QT_USE_NAMESPACE

//! [constructor]
SimpleHttpServer::SimpleHttpServer(const QHostAddress host, const int port,bool debug, QObject *parent) :
    QObject(parent),
    m_host(host),
    m_port(port),
    m_debug(debug)
{
    m_wsServerState=false;
    if (m_debug)
        qDebug() << "Http server:" << host<<",port:"<<port;
   connect(this, &SimpleHttpServer::testId, this, &SimpleHttpServer::onTestId);
   m_simpleHttpServer.listen(m_host,m_port);
   initRoutes(m_port);

}
//! [constructor]
//!
void SimpleHttpServer::initRoutes(int port){
    const int lport=port;
    m_simpleHttpServer.route("/",[port](const QHttpServerRequest &request){
        return QString("The WESINX gateway server is running on port %1").arg(port);
    });

    //包含id的get请求为预留功能，该请求返回缓存在http服务器中的websocket请求
    //如果没有请求则返回 NA
    //返回的请求可能包含两条关于服务器状态的字符串
    //WebSocket Server Offline 服务器离线和 online 在线
    //get返回的消息为字符串格式

    //lambda匿名函数没有办法直接使用外部变量，需要通过函数前面的[]来捕获变量，在类中通过this方式来进行捕获
    m_simpleHttpServer.route("/v1/query/", [this] (qint32 id, const QHttpServerRequest &request) {
        //信号发送后，线程会阻塞，直到槽函数运行结束，虽然信号不能直接接收返回值，但是可以通过引用的方式间接获得结果
        //  emit testId(id);
        if(responseList.isEmpty()){
            return QString("NA");
        }else{
            return responseList.takeFirst();
        }
    });
    m_simpleHttpServer.route("/v1/query/", [this] (const QHttpServerRequest &request) {
        //信号发送后，线程会阻塞，直到槽函数运行结束，虽然信号不能直接接收返回值，但是可以通过引用的方式间接获得结果
        if(responseList.isEmpty()){
            return QString("NA");
        }else{
            return responseList.takeFirst();
        }
    });

    //post到 /v1/sendws/路径的请求中load字段的内容会被转发到WebSocket服务器
    //其他内容暂时未做处理
    //post请求和返回值都为 JSON 格式，其中用户收到服务器的消息与发送消息相同，用于显示服务器收到相关消息
    m_simpleHttpServer.route("/v1/sendws/",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
        const QJsonObject reqObject=byteArrayToJsonObject(request.body());
        if(m_debug){
            qDebug()<<"request is"<<reqObject<<Qt::endl;
        }
        if(reqObject.contains("load")&&reqObject["load"].isString()){
            QString msg=reqObject["load"].toString();
            emit receivePostMsg(msg);
        }
        return QHttpServerResponse(reqObject);
    });
}
//just for test purpose
void SimpleHttpServer::onTestId(int &newId){
    ++newId;
}

//字符串和JSON之间转换
const QJsonObject SimpleHttpServer::byteArrayToJsonObject(const QByteArray &arr)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(arr, &err);
    if (err.error || !json.isObject())
        return QJsonObject{{"err","request parse error"}};
    return json.object();
}

//收到websocket服务器的消息，暂存在服务器消息队列中
void SimpleHttpServer::onReceiveWsResponse(QString &msg)
{
    if(m_debug)
        qDebug()<<"receive ws response:"<<msg<<Qt::endl;
    responseList.append(msg);
}

//收到websocket服务器上下线消息，暂存在服务器消息队列中
void SimpleHttpServer::onWsServerStateChanged(bool &flag){
    m_wsServerState=flag;
    if(!m_wsServerState){
        responseList.append("WebSocket Server Offline");
    }else{
        responseList.append("WebSocket Server Online");
    }
};
