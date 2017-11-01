#ifndef DEVICESETTING_H
#define DEVICESETTING_H
#include <QString>
#include <qdebug.h>
#include <QVector>
#include <QList>
#include <QFile>
#include "pubulicDefine.h"
class DeviceSetting
{
public:
    DeviceSetting();
    static QString deviceID;
    static QString busFd;
    static QString programVID;
    static QString serverIP;
    static QString usbSet;
    static int delaySeconds;
    static int maxDelaySeconds;


    static QList<QString> actionKey;

    void showDeviceInfo();
    void SetDevicePosition();
    void addActionKey();
    void initDevice();
};

#endif // DEVICESETTING_H
