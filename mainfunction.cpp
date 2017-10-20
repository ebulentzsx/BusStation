#include "mainfunction.h"

MainFunction::MainFunction(QObject *parent) :
    QObject(parent)
{
    //pHttpFun=new HttpFun();
    pHttpFun=NULL;
    newInfo=new GetHttpReturn();
    timer = new QTimer();
    dealComInfo=new secondFunction();
    my_com=new myCOM();
    my_com->setCOM();
    my_com->openCOM();
    my_com->recvCOM();


   // QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slot_sendRequest()));
    QObject::connect(newInfo, SIGNAL(signal_writeCom(QString)), this, SLOT(slot_writeCom(QString)));
    QObject::connect(my_com, SIGNAL(signal_getState(QString)), dealComInfo, SLOT(slot_getState(QString)));
    QObject::connect(this,SIGNAL(signal_send_COM(QByteArray)),my_com,SLOT(slot_send_COM(QByteArray)));
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
    //my_com->sendCOM(buf);
    emit signal_send_COM(buf);
 }




void MainFunction::beginLoop()
{

    newInfo->GetUrl(1);
    dealComInfo->start();
    timer->start(5000);
    timer->setSingleShot( false );
}
