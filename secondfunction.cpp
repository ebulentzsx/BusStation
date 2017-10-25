#include "secondfunction.h"

secondFunction::secondFunction(QObject *parent) :
    QObject(parent)
{

     wtach_timer = new QTimer();
     QObject::connect(wtach_timer, SIGNAL(timeout()), this, SLOT(slot_send_get_status()));
     qDebug() << QString("second function :slot and signal connected");

}

void secondFunction::beginLoop()
{

    wtach_timer->start(4000);
    wtach_timer->setSingleShot(false);
    qDebug() << QString("second function thread id:run") << QThread::currentThreadId();
}

void secondFunction::get_all_status()
{
        QByteArray buf,info;
        buf.clear();
        info.clear();
        uint sum=0;
        buf[0]=0x68;
        for(int i=1;i<7;++i)
            buf[i]=0x99;
        buf[7]=0x68;
        buf[8]=0x01;
        buf[9]=2;
        buf[10]=(0x34+0x33)%256;
        buf[11]=(0xaa+0x33)%256;
        //qDebug()<<"Go to write the COM";
        for(int k=0;k<12;++k)
        {
            sum=sum+buf[k];
        }
        buf[12]=sum%256;
        buf[13]=0x16;
        //deal the information of bus which update now
        qDebug()<<"send to com:slot_get_allstatus"<<buf.toHex();

      // emit signal_send_com(buf);
}

void secondFunction::updateToServer()
{
    secHttp=new HttpFun();
    QObject::connect(secHttp,SIGNAL(signal_requestFinished(bool,QString)),this,SLOT(slot_requestFinished(bool,QString)));
    secHttp->sendRequest(post_url);
}
void secondFunction::slot_getState(const QString &strResult)
{
    //QByteArray info;
    tmp_HardwareInfo.clear();
    tmp_HardwareInfo.append(strResult);
    //info.append(strResult);
    qDebug() << QString("slot in second function thread id:get state") << QThread::currentThreadId();
    qDebug()<<"second function get state from COM"<<tmp_HardwareInfo;
    //get the URL
    //POST and update
    //sleep(2);

}

void secondFunction::slot_send_get_status()
{

    busDev.showDeviceInfo();
    qDebug() << QString("slot in second function thread id:slot_send_get_status") << QThread::currentThreadId();
    get_all_status();
}

void secondFunction::slot_init_watch()
{
    beginLoop();
    qDebug() << QString("slot in second function thread id:slot_init_watch") << QThread::currentThreadId();
}

void secondFunction::slot_requestFinished(bool bSuccess, const QString &strResult)
{
    if(bSuccess)
    {

    }
    else
    {


    }
}
