#include "devicesetting.h"
QString DeviceSetting::deviceID=NULL;
QString DeviceSetting::busFd=NULL;
QString DeviceSetting::programVID=NULL;
QString DeviceSetting::serverIP="http://123.207.75.109:10000/YiYangIndex.ashx?ActionKey=";
QList<QString>  DeviceSetting::actionKey;
DeviceSetting::DeviceSetting()
{
    qDebug()<<"DeviceSetting new finished!";
}

void DeviceSetting::showDeviceInfo()
{
    qDebug()<<"Device ID:"<<DeviceSetting::deviceID;
    qDebug()<<"Program version:"<<DeviceSetting::programVID;
    qDebug()<<"Server IP:"<<DeviceSetting::serverIP;
    foreach (QString x,DeviceSetting::actionKey) {
        qDebug()<<x;
    }
}
