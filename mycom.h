#ifndef MYCOM_H
#define MYCOM_H
#include <QtCore>
#include <QObject>
#include "qextserialbase.h"
#include "posix_qextserialport.h"
#include "readcomthread.h"
#include <QCoreApplication>
class myCOM : public QObject
{
    Q_OBJECT
public:
    explicit myCOM(QObject *parent = 0);
    readComThread *readThread;
    Posix_QextSerialPort *myCom;
    QString strTime;
    bool flag_isOpen;
  //  myCOM();
    void setCOM();//Setting
    int openCOM();//Open COM and create new thread
    int sendCOM(QByteArray buf);
    int recvCOM();
    int testCOM();
    void closeCOM();
    void getCurrentTime();
signals:
    void signal_getErrorMsg(int code, const QString &strResult);
    void signal_getState(const QString &strResult);
public slots:
    void slot_getStateFromCom(const QString &tmp);
};

#endif // MYCOM_H
