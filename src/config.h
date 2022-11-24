#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QJsonArray>

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(const QString configDir,bool debug = false, QObject *parent = nullptr);
    bool loadConfigFile(QString dir);
    QJsonObject getconf();
    QJsonValue findConf(QString key,QJsonValue defaultValue);
    bool setConf(QString key,QJsonValue setValue);
    bool delConf(QString key);
    bool resetConf();

Q_SIGNALS:
    void configError();

private:
    QString m_dir;
    bool m_debug;
    QJsonObject configObj;
    inline static const QString configFileName{"config.json"};
    inline static const QString defaultConfigFileName{"config.default.json"};
    bool loadConfigFromFile();
    bool parseToJson(QByteArray data);
    bool saveToConfigFile();
};



#endif // CONFIG_H
