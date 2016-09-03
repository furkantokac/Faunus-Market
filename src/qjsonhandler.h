#ifndef QJSONHANDLER_H
#define QJSONHANDLER_H

#include <QObject>

#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include <QStringList>

#include <QFile>
#include <QDebug>

class QJsonHandler : public QObject
{
    Q_OBJECT
public:
    explicit QJsonHandler(QObject *parent = 0);

    bool save();
    bool load(QString fileName);

    void setStringList(QString key, QStringList value);
    void setJsonArray(QString key, QJsonArray value);
    void set(QString key, QString value);

    QStringList getStringList(QString key);
    QJsonArray getJsonArray(QString key);
    QString get(QString key);
    QString get(QString key, int i);

    void append(QString key, QString value);
    bool remove(QString key, QString value);

    void reset(QString key);
    void reset(QString key, QJsonValue initVal);

    bool isFileLoaded=false;
    QString fileName;

signals:

public slots:

private:
    QJsonObject json;
};

#endif // QJSONHANDLER_H
