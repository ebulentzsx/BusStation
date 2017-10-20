#include "readcomthread.h"
#include <QDebug>
#include <QtCore>
readComThread::readComThread(QObject *parent) :
    QThread(parent)
//readComThread::readComThread()
{
    qDebug() << QString("readthread class Making id:") << QThread::currentThreadId();
    stopRead=false;
}

void readComThread::run()
//keep redding
//exit conditions: flagstop=true or COM is closed
{

   // qDebug()<<"running";
    qDebug() << QString("read thread id:") << QThread::currentThreadId();
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
            qDebug()<<"Read OK!";
            emit signal_getStateFromCom(temp);
            qDebug()<<"Receive:"<<temp.toHex();

            temp.clear();
        }
        //msleep(200);
       // qDebug()<<"whileing~~~";
    }
    //qDebug()<<"running endings"<<stopRead;
}

void readComThread::setCom(Posix_QextSerialPort *com)
{
    qDebug() << QString("readthread id:setCom") << QThread::currentThreadId();
    readCom=com;
}

void readComThread::setStop()
{
    qDebug() << QString("readthread id:setStop") << QThread::currentThreadId();
    stopRead=true;
}
