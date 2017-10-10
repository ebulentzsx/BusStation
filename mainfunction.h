#ifndef MAINFUNCTION_H
#define MAINFUNCTION_H

#include <QObject>
#include "HttpFun.h"
#include "gethttpreturn.h"
class MainFunction : public QObject
{
    Q_OBJECT
public:
    explicit MainFunction(QObject *parent = 0);
    HttpFun *pHttpFun;
    GetHttpReturn *newInfo;
    QTimer *timer;
    void setSys_time();
    void beginLoop();

signals:
    void signal_sendRequest(const QString& strUrl);
public slots:
    void slot_sendRequest();
private:

};

#endif // MAINFUNCTION_H
