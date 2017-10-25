#ifndef DEVICESETTING_H
#define DEVICESETTING_H
#include <QString>
#include <qdebug.h>
#include <QVector>
#include <QList>
class DeviceSetting
{
public:
    DeviceSetting();
    static QString deviceID;
    static QString busFd;
    static QString programVID;
    static QString serverIP;

    static QList<QString> actionKey;

    void showDeviceInfo();
};

#endif // DEVICESETTING_H
