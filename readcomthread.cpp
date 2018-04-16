#include "readcomthread.h"
#include <QDebug>
#include <QtCore>
readComThread::readComThread(QObject *parent) :
    QThread(parent)
//readComThread::readComThread()
{

    stopRead=false;
}

void readComThread::run()
//keep redding
//exit conditions: flagstop=true or COM is closed
{

   // qDebug()<<"running";
    qDebug() << QString("read thread id:run") << QThread::currentThreadId();
    QByteArray temp;
    while(1){
        //qDebug()<<"running in wihile in run"<<stopRead;
        if(stopRead || (readCom->isOpen()==false))
        {
            qDebug()<<"Break!!!";
            break;
        }
        temp= readCom->readAll();
        if(!temp.isEmpty()){
            qDebug()<<"Read OK!Receive:"<<temp.toHex();

            emit signal_getStateFromCom(temp);

            temp.clear();
        }
        //msleep(200);
       // qDebug()<<"whileing~~~";
    }
    //qDebug()<<"running endings"<<stopRead;
}

void readComThread::setCom(Posix_QextSerialPort *com)
{

    readCom=com;
}

void readComThread::setStop()
{

    stopRead=true;
}
