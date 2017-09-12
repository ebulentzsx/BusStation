#ifndef READCOMTHREAD_H
#define READCOMTHREAD_H
#include <QtCore>
#include "qextserialbase.h"
#include "posix_qextserialport.h"
#include <QTextStream>
#include <QCoreApplication>
class readComThread : public QThread
{
private:

public:
    bool stopRead;//flag
    Posix_QextSerialPort *readCom;//com addr&information

    readComThread();
    void run();//Start read from COM
    void setStop(); //Set flag to stop
    void setCom(Posix_QextSerialPort *com); //set com

};

#endif // READCOMTHREAD_H
