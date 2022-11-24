#include "config.h"

QT_USE_NAMESPACE

//! [constructor]
Config::Config(const QString configDir,bool debug, QObject *parent):
    QObject(parent),
    m_dir(configDir),
    m_debug(debug)
{
    if (m_debug)
        qDebug() << "Config File is" <<m_dir<<"config.json"<<Qt::endl;
    loadConfigFromFile();

}

//如果读取配置文件失败，则读取默认的配置文件
bool Config::loadConfigFromFile(){
    QFile file(configFileName);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text)|!parseToJson(file.readAll())){
        if(m_debug)
            qDebug()<<"Config file error,try default config file";
        file.close();
        QFile file2(defaultConfigFileName);
        if(!file2.open(QIODevice::ReadOnly|QIODevice::Text)|!parseToJson(file2.readAll())){
            if(m_debug)
                qDebug()<<"Default Config file error";
            emit configError();
            return false;
        }
        if(m_debug)
            qDebug()<<"Default Config File will be used to reset config";
        file2.close();
        return true;
    }
    file.close();
    return true;
}

//将配置文件转换为JSON对象
bool Config::parseToJson(QByteArray data){
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data,&jsonError));
    if(jsonError.error || !jsonDoc.isObject()){
        if(m_debug)
            qDebug()<<"parse json error"<<Qt::endl;
        return false;
    }
    configObj=jsonDoc.object();
    if(m_debug){
    QStringList keys=configObj.keys();
    for(int i=0;i<keys.size();++i)
        qDebug()<<"key:"<<keys.at(i)<<",value:"<<configObj[keys.at(i)];
    }
    return true;
}

//获取配置文件
QJsonObject Config::getconf(){
    return configObj;
};

//查找配置文件项，如果找不到则用默认值
QJsonValue Config::findConf(QString key,QJsonValue defaultValue){
    if(configObj.contains(key)&&configObj[key].type()==defaultValue.type()){
        return configObj[key];
    }
    return defaultValue;
}

//使用默认配置文件重置配置
bool Config::resetConf(){
    QFile file;
    if(file.exists(configFileName)&&file.remove(configFileName)){
        file.copy(defaultConfigFileName,configFileName);
        return loadConfigFromFile();
    }
    return false;
}

//设置配置文件值，如果没有键则新建一个
bool Config::setConf(QString key,QJsonValue setValue)
{
    if(configObj.contains(key)&&setValue.type()==configObj[key].type()){
        configObj[key]=setValue;

    }else{
        configObj.insert(key,setValue);
    }
    return saveToConfigFile();
};

//删除配置项
bool Config::delConf(QString key){
    if(configObj.contains(key)){
        configObj.remove(key);
        return saveToConfigFile();
    }
    return false;
};


bool Config::saveToConfigFile(){
    QFile file(configFileName);
    QJsonDocument doc(configObj);
    if(!file.open(QIODevice::WriteOnly|QIODeviceBase::Truncate)){
        if(m_debug)
            qDebug()<<"Config file error,try default config file";
        file.close();
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
};
