#ifndef SECONDFUNCTION_H
#define SECONDFUNCTION_H

#include <QThread>
#include <QtCore>
#include <QTimer>
#include "devicesetting.h"
#include "HttpFun.h"
class secondFunction : public QObject
{
    Q_OBJECT
public:
    QTimer *wtach_timer;
    explicit secondFunction(QObject *parent = 0);
    void beginLoop();
    void get_all_status();
    void updateToServer();
    QString post_url;
    QByteArray tmp_HardwareInfo;
    HttpFun *secHttp;
    DeviceSetting busDev;
    //DeviceSetting busDev;
signals:
    void signal_send_com(QByteArray tmp);
public slots:
    void slot_getState(const QString &strResult);
    void slot_send_get_status();
    void slot_init_watch();
    void slot_requestFinished(bool bSuccess, const QString &strResult);

};

#endif // SECONDFUNCTION_H
