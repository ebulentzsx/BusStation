#ifndef MYCOM_H
#define MYCOM_H
#include <QtCore>
#include "qextserialbase.h"
#include "posix_qextserialport.h"
#include "readcomthread.h"
#include <QCoreApplication>
class myCOM
{

public:
    readComThread readThread;
    Posix_QextSerialPort *myCom;
    QString strTime;
    bool flag_isOpen;
    myCOM();
    void setCOM();//Setting
    int openCOM();//Open COM and create new thread
    int sendCOM(QByteArray buf);
    int recvCOM();
    int testCOM();
    void closeCOM();
    void getCurrentTime();
signals:
    void signal_getErrorMsg(int code, const QString &strResult);
};

#endif // MYCOM_H
