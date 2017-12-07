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
    QObject::connect(newInfo, SIGNAL(signal_writeCom(QByteArray)), this, SLOT(slot_writeCom(QByteArray)));
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

      qDebug() << QString("get http return thread id:slot_sendRequest") << QThread::currentThreadId();
     if(newInfo->deal_all_finish==false)
     {
         qDebug() << QString("-----------------------------Get new information from server-********************************************8-----------------------------------------") ;
         timer->start(REQUEST_INTERVERL);
         return;
     }
     if(checkTime()==false)
     {
         feedWtachDog();
         sleep(10);
         //newInfo->initAll1096();
         if(newInfo->init_flag==false)
         {
            newInfo->initAll1096();
            newInfo->init_flag=true;
         }
         timer->start(REQUEST_INTERVERL);
         return;
     }
#if DEBUG_GET_ONE_STATION_FROM_CQ
     newInfo->GetUrl(3);
#else
     newInfo->GetUrl(1);
#endif
     pHttpFun=new HttpFun();
     QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
     pHttpFun->sendRequest(newInfo->strUrl);
    qDebug()<<"QTime startinf~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
     timer->start(REQUEST_INTERVERL);
 }

 void MainFunction::slot_writeCom(QByteArray tmp)
 {

    QByteArray buf;
    buf.append(tmp);
    qDebug()<<"in write of main function"<<tmp.toHex();
    qDebug() << QString("slot in main function thread id: slot_writeCom") << QThread::currentThreadId();
    emit signal_send_COM(tmp);
     myCOM::waitCount ++;
 }




void MainFunction::beginLoop()
{

    setSys_time();
    timer->start(REQUEST_INTERVERL);
    timer->setSingleShot( true );

    emit signal_init_COM();

    emit signal_init_watch();
    qDebug() << QString("main loop begin!!!") ;
    initWatchDog();

}

void MainFunction::showTitle()
{

    QByteArray buf,info;
    buf.clear();
    info.clear();
    u_int8_t sum;
    info.append(" ");
    info.append(0xcf);
    info.append(0xdf);//xian--
     info.append(" ");
    info.append(0xc2);
    info.append(0xb7);//lu--
    info.append("-");

#if DEBUG_SHOW_DISTANCE
    //info.append(newBus.BCNO);
    info.append(0xca);
    info.append(0xa3);//sheng
    info.append(0xd3);
    info.append(0xe0);//yu--
    info.append(0xd5);
    info.append(0xbe);//zhan
    info.append("-");
    info.append(0xca);
    info.append(0xa3);//sheng
    //info.append(" ");
    info.append(0xd3);
    info.append(0xe0);//yu--
    //info.append(" ");
    info.append(0xbe);
    info.append(0xe0);//ju
    //info.append(" ");
    info.append(0xc0);
    info.append(0xeb);//li
#else
    info.append(0xd6);
    info.append(0xd5);
    info.append(0xb5);
    info.append(0xe3);
    info.append(0xd5);
    info.append(0xbe);//zhan
    info.append("-");
    info.append(0xca);
    info.append(0xa3);//sheng
    info.append(0xd3);
    info.append(0xe0);//yu--
    info.append(0xd5);
    info.append(0xbe);//zhan
#endif
/* shengyuzhan shengyujuli

*/

    //D5BE zhan
    //CAA3 sheng
    //CFDF xian
    //C2b7 lu
    //D5BE zhan
    //BEE0  ju
    //C0EB  li
    //D3E0 yu
    //D6D5 zhong
    //B5E3 dian
    //info.right()
    //info.append("-");
    //info.append("-");

    //info.append("/n");
    uint len=info.length();
    //qDebug()<<"info.length:"<<len;
    //int len=info.count();
   qDebug()<<"Write the COM info:"<<info;
   // int len=newBus.BRNO.length()+newBus.BCNO.length()+newBus.CD.length();
    buf[0]=0x68;
    for(int i=1;i<7;++i)
        buf[i]=0x99;
    buf[7]=0x68;
    buf[8]=0x04;
    buf[9]=1+2+len;
    buf[10]=0x66;
    buf[11]=0xdd;

    buf[12]=0x01+0x33;


    //number of screen
    //waitting for further development
    //qDebug()<<"Go to write the COM";
    //info.append(QString::number(newBus.id));
    //info.append(" ");

   // info.append(" ");

    for(int j=0;j<len;++j)
    {
        buf[13+j]=(info[j]+0x33)%256;
    }
    for(int k=0;k<(13+len);++k)
    {
        sum=(sum+buf[k])%256;
    }
    buf[13+len]=sum;
    buf[13+len+1]=0x16;
    //deal the information of bus which update now
    qDebug()<<"send to com:getCOM_buf"<<buf.toHex();
    emit signal_send_COM(buf);
    myCOM::waitCount ++;
     buf[12]=buf[12]+0x0a;
     buf[13+len]=buf[13+len]+0x0a;
     sleep(3);
     emit signal_send_COM(buf);
     myCOM::waitCount ++;
     sleep(3);

}

void MainFunction::showNULL()
{
    QByteArray buf,info;
    buf.clear();
    info.clear();
    u_int8_t sum;
    info.append("    ");
    uint len=info.length();
    //qDebug()<<"info.length:"<<len;
    //int len=info.count();
   qDebug()<<"Write the COM info:"<<info;
   // int len=newBus.BRNO.length()+newBus.BCNO.length()+newBus.CD.length();
    buf[0]=0x68;
    for(int i=1;i<7;++i)
        buf[i]=0x99;
    buf[7]=0x68;
    buf[8]=0x04;
    buf[9]=1+2+len;
    buf[10]=0x66;
    buf[11]=0xdd;
    buf[12]=0x01+0x33;
    for(int j=0;j<len;++j)
    {
        buf[13+j]=(info[j]+0x33)%256;
    }
    for(int k=0;k<(13+len);++k)
    {
        sum=(sum+buf[k])%256;
    }
    buf[13+len]=sum;
    buf[13+len+1]=0x16;
    //deal the information of bus which update now

    int i=0;
    while(i<20)
    {
        emit signal_send_COM(buf);
        qDebug()<<"send to com:showNULL"<<buf.toHex()<<"---int i---"<<i+1;
        buf[12]=buf[12]+0x01;
        buf[13+len]=buf[13+len]+0x01;
        i++;
        sleep(3);
    }

}

bool MainFunction::checkTime()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("h");

    int intTime=strTime.toInt();
    qDebug()<<"Current time ------------------------------:::::::"<<strTime<<"-----int"<<intTime;
    if(intTime==23 || intTime<5)
        return false;
    else
        return true;

}

void MainFunction::initWatchDog()
{
     system("echo 0 >/dev/watchdog");
}
void MainFunction::feedWtachDog()
{
     system("echo 0 >/dev/watchdog");
}



