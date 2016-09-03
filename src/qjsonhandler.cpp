#include "qjsonhandler.h"

QJsonHandler::QJsonHandler(QObject *parent) : QObject(parent)
{
}

bool QJsonHandler::load(QString fn)
{
    if( !QFile(fn).exists() )
    {
        qDebug() << fn+" file is not exists.";
        return false;
    }

    QFile loadFile(fn);
    if( !loadFile.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Unable to open file.";
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument myDoc = QJsonDocument::fromJson(saveData);
    json = myDoc.object();

    fileName = fn;
    isFileLoaded = true;
    return true;
}

bool QJsonHandler::save()
{
    if( !isFileLoaded )
    {
        qDebug() << "Cannot save file without open it.";
        return false;
    }

    QFile saveFile(fileName);
    if( !saveFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to open file to save.";
        return false;
    }

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    return true;
}

// ---BEG GET FUNCTIONS
// Returns json[key] as stringList
QStringList QJsonHandler::getStringList(QString key)
{
    QJsonArray jsonArray = json[key].toArray();
    QStringList strList;

    for(int i=0; i<jsonArray.size(); i++)
        strList.append(jsonArray[i].toString());

    return strList;
}

// Returns json[key] as jsonArray
QJsonArray QJsonHandler::getJsonArray(QString key)
{
    return json[key].toArray();
}

// Returns first item of the jsonArray. array[0]
QString QJsonHandler::get(QString key)
{
    return json[key].toArray()[0].toString();
}

// Returns i. item of the jsonArray. array[i]
QString QJsonHandler::get(QString key, int i)
{
    return json[key].toArray()[i].toString();
}

// ---END GET FUNCTIONS


// ---BEG SET FUNCTIONS
void QJsonHandler::setJsonArray(QString key, QJsonArray value)
{
    json[key] = value;
}

void QJsonHandler::setStringList(QString key, QStringList value)
{
    QJsonArray jsonArray;

    for(int i=0; i<value.size(); i++)
        jsonArray.append(value[i]);

    setJsonArray(key, jsonArray);
}

void QJsonHandler::set(QString key, QString value)
{
    QJsonArray tempArray = json[key].toArray();
    tempArray[0] = value;
    json[key] = tempArray;
}
// ---END SET FUNCTIONS


void QJsonHandler::append(QString key, QString value)
{
    QJsonArray tempArray = json[key].toArray();
    tempArray.append(value);
    json[key] = tempArray;
}

bool QJsonHandler::remove(QString key, QString value)
{
    QJsonArray jsonArray = json[key].toArray();
    int i=0;

    for(i=0; i<jsonArray.size(); i++)
        if(jsonArray[i].toString()==value)
        {
            jsonArray.removeAt(i);
            setJsonArray(key, jsonArray);
            return true;
        }

    return false;
}

void QJsonHandler::reset(QString key)
{
    QJsonArray emptyArray;
    emptyArray.append("");
    json[key] = emptyArray;
}

void QJsonHandler::reset(QString key, QJsonValue initVal)
{
    QJsonArray emptyArray;
    emptyArray.append(initVal);
    json[key] = emptyArray;
}
