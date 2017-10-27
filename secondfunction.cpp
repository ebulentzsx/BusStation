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

    wtach_timer->start(60000);
    wtach_timer->setSingleShot(false);
    qDebug() << QString("second function thread id:run") << QThread::currentThreadId();
}

void secondFunction::get_all_status()
{
        QByteArray buf,info;
        buf.clear();
        info.clear();
        u_int8_t sum=0;
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
            sum=(sum+buf[k])%256;
        }
        buf[12]=sum;
        buf[13]=0x16;
        //deal the information of bus which update now
        qDebug()<<"send to com:slot_get_allstatus"<<buf.toHex();

        emit signal_send_com(buf);
}

void secondFunction::updateToServer()
{
    secHttp=new HttpFun();
    QObject::connect(secHttp,SIGNAL(signal_requestFinished(bool,QString)),this,SLOT(slot_requestFinished(bool,QString)));
    secHttp->sendRequest(post_url);
}
void secondFunction::slot_getState(const QString &strResult)
{
    QByteArray info;
    info.append(strResult);
   // qDebug() << QString("---------------slot in second function thread id:get state") <<info[8]<<QThread::currentThreadId();
     //qDebug() << QString("---------------slot in second function thread id:get state") <<info;
    //qDebug()<<"second function get state from COM"<<tmp_HardwareInfo;
    //qDebug() << QString("slot in mycom thread id:get state") << QThread::currentThreadId();

     if (( 0x68==info[0])&&(0x68== info[7] )&&(0x81==info[8]))
         qDebug() <<"---------------------Get all status:Return OK"<<info;
     if (( 0x68==info[0])&&(0x68== info[7] )&&(0x84==info[8]))
         qDebug() <<"---------------------Control display:Return OK"<<info;


}

void secondFunction::slot_send_get_status()
{

    //busDev.showDeviceInfo();
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
