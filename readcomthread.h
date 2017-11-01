#ifndef READCOMTHREAD_H
#define READCOMTHREAD_H
#include <QtCore>
#include "qextserialbase.h"
#include "posix_qextserialport.h"
#include <QTextStream>
#include <QCoreApplication>
#include "pubulicDefine.h"
class readComThread : public QThread
{
    Q_OBJECT
private:

public:
    //readComThread();
    explicit readComThread(QObject *parent = 0);
    bool stopRead;//flag
    Posix_QextSerialPort *readCom;//com addr&information

  //  QThread readCom();
    void run();//Start read from COM
    void setStop(); //Set flag to stop
    void setCom(Posix_QextSerialPort *com); //set com
signals:
    void signal_getStateFromCom(const QString &strResult);
};

#endif // READCOMTHREAD_H
