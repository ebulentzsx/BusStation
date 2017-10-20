#include "mainfunction.h"

MainFunction::MainFunction(QObject *parent) :
    QObject(parent)
{
    //pHttpFun=new HttpFun();
    pHttpFun=NULL;
    newInfo=new GetHttpReturn();
    timer = new QTimer();
    my_com=new myCOM();
    my_com->setCOM();
    my_com->openCOM();
    my_com->recvCOM();
   // QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slot_sendRequest()));
    QObject::connect(newInfo, SIGNAL(signal_writeCom(QString)), this, SLOT(slot_writeCom(QString)));
    QObject::connect(my_com, SIGNAL(signal_getState(QString)), this, SLOT(slot_getState(QString)));
}
void MainFunction::setSys_time()
{
    newInfo->GetUrl(2);
    pHttpFun=new HttpFun();
    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(newInfo->strUrl);
}
 void MainFunction::slot_sendRequest()
 {
     pHttpFun=new HttpFun();
     QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
     pHttpFun->sendRequest(newInfo->strUrl);

 }

 void MainFunction::slot_writeCom(const QString &tmp)
 {
    QByteArray buf;
    buf.append(tmp);
    qDebug()<<"in write function"<<buf.toHex();
    my_com->sendCOM(buf);
 }

 void MainFunction::slot_getState(const QString &strResult)
 {
     QByteArray info;
     info.append(strResult);
     qDebug()<<"get state from com"<<info;
 }



void MainFunction::beginLoop()
{

    newInfo->GetUrl(1);
    timer->start(5000);
    timer->setSingleShot( false );
}
