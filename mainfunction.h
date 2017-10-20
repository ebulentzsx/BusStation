#ifndef MAINFUNCTION_H
#define MAINFUNCTION_H

#include <QObject>
#include "HttpFun.h"
#include "gethttpreturn.h"
#include "mycom.h"
#include "busline.h"
#include "secondfunction.h"
class MainFunction : public QObject
{
    Q_OBJECT
public:
    explicit MainFunction(QObject *parent = 0);
    HttpFun *pHttpFun;
    GetHttpReturn *newInfo;
    QTimer *timer;
    myCOM *my_com;
    secondFunction *dealComInfo;
    void setSys_time();
    void beginLoop();

signals:
    void signal_sendRequest(const QString& strUrl);
    void signal_send_COM(QByteArray buf);
public slots:
    void slot_sendRequest();
    void slot_writeCom(const QString& strResult);

private:

};

#endif // MAINFUNCTION_H