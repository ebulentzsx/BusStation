#include "mainfunction.h"

MainFunction::MainFunction(QObject *parent) :
    QObject(parent)
{
    //pHttpFun=new HttpFun();
    pHttpFun=NULL;
    newInfo=new GetHttpReturn();
    timer = new QTimer();
    //dealComInfo=new secondFunction();
   // my_com=new myCOM();
   // my_com->moveToThread(dealComInfo);

   // my_com->setCOM();
    //my_com->openCOM();
   // my_com->recvCOM();

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slot_sendRequest()));
    QObject::connect(newInfo, SIGNAL(signal_writeCom(QString)), this, SLOT(slot_writeCom(QString)));
   // QObject::connect(my_com, SIGNAL(signal_getState(QString)), dealComInfo, SLOT(slot_getState(QString)));
    //QObject::connect(this,SIGNAL(signal_send_COM(QByteArray)),my_com,SLOT(slot_send_COM(QByteArray)));
    //QObject::connect(dealComInfo,SIGNAL(signal_send_cmd()),newInfo,SLOT(slot_get_allstatus()));
}
void MainFunction::setSys_time()
{
    newInfo->GetUrl(2);
    pHttpFun=new HttpFun();
    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(newInfo->strUrl);
    qDebug() << QString("slot in main function thread id:setSys_time") << QThread::currentThreadId();
}
 void MainFunction::slot_sendRequest()
 {
     newInfo->GetUrl(1);
     pHttpFun=new HttpFun();
     QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
     pHttpFun->sendRequest(newInfo->strUrl);
     qDebug() << QString("get http return thread id:slot_sendRequest") << QThread::currentThreadId();

 }

 void MainFunction::slot_writeCom(const QString &tmp)
 {

    QByteArray buf;
    buf.append(tmp);
    qDebug()<<"in write of main function"<<buf.toHex();
    qDebug() << QString("slot in main function thread id: slot_writeCom") << QThread::currentThreadId();
    emit signal_send_COM(buf);
 }




void MainFunction::beginLoop()
{

    //setSys_time();
    timer->start(3000);
    timer->setSingleShot( false );
    emit signal_init_COM();
    emit signal_init_watch();
    qDebug() << QString("main loop begin!!!") ;

}

