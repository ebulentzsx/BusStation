#ifndef SECONDFUNCTION_H
#define SECONDFUNCTION_H

#include <QThread>
#include <QtCore>
#include <QTimer>
#include "devicesetting.h"
#include "HttpFun.h"
#include "pubulicDefine.h"
#include "mainfunction.h"
class secondFunction : public QObject
{
    Q_OBJECT
public:
    QTimer *wtach_timer;
    explicit secondFunction(QObject *parent = 0);
    void beginLoop();
    void get_all_status();
    void updateToServer();
    void updateAllToServer();
    void postSunLog();
    void postBatteryLog();
    static int delaySeconds;
    static int maxDelaySeconds;
    int secTimer;
    QString post_url;
    QByteArray tmp_HardwareInfo;
    HttpFun *secHttp;
    DeviceSetting busDev;
    bool wetherFinished;
    //DeviceSetting busDev;
signals:
    void signal_send_com(QByteArray tmp);
public slots:
    void slot_getState(const QString &strResult);
    void slot_init_watch();
    void slot_requestFinished(bool bSuccess, const QString &strResult);
    void  slot_pubulic_timer();

};

#endif // SECONDFUNCTION_H
