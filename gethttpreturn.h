#ifndef GETHTTPRETURN_H
#define GETHTTPRETURN_H
#include <qprocess.h>
#include <QObject>
#include <qdebug.h>
#include <qlinkedlist.h>
#include "busline.h"
#define GET_BUS_IFOR 1
#define GET_SYS_TIME 2
#define SERVER_IP "http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey="

class GetHttpReturn : public QObject
{
    Q_OBJECT
public:
    int p_cmdFlag;
    explicit GetHttpReturn(QObject *parent = 0);
    QLinkedList <BusLine> lineList;
    void GetLines();
    void GetUrl(int cmdFlag);
    void SetDevicePosition(QString deviceId,QString bufForword);
    void SetSysTime();
    QString strInfor;
    QString strUrl;
    QString deviceID;
    QString busFd;
signals:

public slots:

    void slot_requestFinished(bool bSuccess, const QString &strResult);

};

#endif // GETHTTPRETURN_H
