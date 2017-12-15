#ifndef MAINFUNCTION_H
#define MAINFUNCTION_H

#include <QObject>
#include "HttpFun.h"
#include "gethttpreturn.h"
#include "mycom.h"
#include "busline.h"
#include "secondfunction.h"
#include "pubulicDefine.h"
class MainFunction : public QObject
{
    Q_OBJECT
public:
    explicit MainFunction(QObject *parent = 0);
    HttpFun *pHttpFun;
    GetHttpReturn *newInfo;
    QTimer *timer;


    //secondFunction *dealComInfo;
    void setSys_time();
    void beginLoop();
    void run();
    void showTitle();
    void showNULL();
    bool checkTime();
    void initWatchDog();
    void feedWtachDog();

signals:
    void signal_sendRequest(const QString& strUrl);
    void signal_send_COM(QByteArray buf);
    void signal_init_COM();
    void signal_init_watch();
public slots:
    void slot_sendRequest();
    void slot_writeCom(QByteArray strResult);
    void slot_startTimer();

private:

};

#endif // MAINFUNCTION_H
