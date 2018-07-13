#ifndef DEVICESETTING_H
#define DEVICESETTING_H
#include <QString>
#include <qdebug.h>
#include <QVector>
#include <QList>
#include <QFile>
#include <QStringList>
#include "pubulicDefine.h"
class DeviceSetting
{
public:
    DeviceSetting();
    static QString deviceID;
    //static QString busFd;
    static QString serverIP;
    static QString usbSet;
    static QString stationCode;
    static QString hostIP;
    static QString picVersion;
    static QString appVersion;
    static int delaySeconds;
    static int maxDelaySeconds;
    static int error_Reboot;
    static int com_error_Reboot;
    static int serverNoUpdate;
    static int errCode;
    static int updateBegin;



    static QList<QString> actionKey;

    void showDeviceInfo();
    void SetDevicePosition();
    void addActionKey();
    void initDevice();
    void intAppVersion(QString tmpVersion);
    void setConfigTxt(QString keyString,QString inputString);
};

#endif // DEVICESETTING_H
