#include "mainfunction.h"

MainFunction::MainFunction(QObject *parent) :
    QObject(parent)
{

    pHttpFun=NULL;
    newInfo=new GetHttpReturn();
    timer = new QTimer();


    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slot_sendRequest()));
    QObject::connect(newInfo, SIGNAL(signal_writeCom(QByteArray)), this, SLOT(slot_writeCom(QByteArray)));
    QObject::connect(newInfo, SIGNAL(signal_startTimer()), this, SLOT(slot_startTimer()));
}



void MainFunction::setSys_time()
{
    newInfo->GetUrl(GET_SYS_TIME);
    pHttpFun=new HttpFun();
    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(newInfo->strUrl);
    qDebug() << QString("setSys_time-----------slot in main function thread id:setSys_time") << QThread::currentThreadId();
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
    //-----------------20180710 Debug test
    if(DeviceSetting::updateBegin==1)
    {
        feedWtachDog();
        getVersionInfo();
        DeviceSetting::updateBegin=0;
        timer->start(50000);
        return;
    }
    //----------------20180710 Debug test
    if(checkTime()==false)
    {
        feedWtachDog();

        //newInfo->initAll1096();
        if(newInfo->init_flag==false)
        {
            newInfo->initAll1096();
            newInfo->init_flag=true;
            // check and update from SERVER
            //http://123.207.239.144:10000/YiYangIndex.ashx?ActionKey=GUGBDN&stationCode=G0473
        }
        newInfo->heartbeatWhenSleep();
        //sleep(30);
        timer->start(30000);
        return;
    }
    if(DeviceSetting::serverNoUpdate==0 || DeviceSetting::serverNoUpdate==2)
    {
        newInfo->GetUrl(1);

        pHttpFun=new HttpFun();
        QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
        pHttpFun->sendRequest(newInfo->strUrl);
    }
    //DeviceSetting::serverNoUpdate==1  Found server noupdate,record to server
    //DeviceSetting::serverNoUpdate==2  Have known update,keep upadte acation.set to 2 after recording
    //DeviceSetting::serverNoUpdate==3  Found server noupdate rescue,record to server.set to 3 after recording(rescue)
    //DeviceSetting::serverNoUpdate==0  Normall state

    else  if(DeviceSetting::serverNoUpdate==1)
    {

        newInfo->GetUrl(SERVER_NO_UPDATE);
        pHttpFun=new HttpFun();
        QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
        pHttpFun->sendRequest(newInfo->strUrl);
        DeviceSetting::serverNoUpdate=2;
    }
    else  if(DeviceSetting::serverNoUpdate==3)
    {
        DeviceSetting::serverNoUpdate=0;
        newInfo->GetUrl(SERVER_NO_UPDATE);
        pHttpFun=new HttpFun();
        QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
        pHttpFun->sendRequest(newInfo->strUrl);
    }

    //timer->start(REQUEST_INTERVERL);
}

void MainFunction::slot_writeCom(QByteArray tmp)
{

    QByteArray buf;
    buf.append(tmp);
    //qDebug()<<"in write of main function"<<tmp.toHex();
    qDebug() << QString("slot in main function thread id: slot_writeCom") << QThread::currentThreadId();
    emit signal_send_COM(tmp);
    // myCOM::waitCount ++;
}

void MainFunction::slot_startTimer()
{

    timer->start(REQUEST_INTERVERL);


    qDebug()<<"QTime starting~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}




void MainFunction::beginLoop()
{

    setSys_time();
    //sleep(3);
    DeviceSetting::updateBegin=1;
    //getVersionInfo();
    checkNewVersion();
    // firstHeartBeat();
    timer->start(REQUEST_INTERVERL);
    timer->setSingleShot( true );

    emit signal_init_COM();

    emit signal_init_watch();
    qDebug() << QString("main loop begin!!!") ;
    initWatchDog();

}






bool MainFunction::checkTime()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("h");

    int intTime=strTime.toInt();
    QString strMinute=time.toString("m");
    int intMinute=strMinute.toInt();
    if(intTime==intMinute)
    {
        system("/root/1063 2 1063.bmp&");
    }
    qDebug()<<"Current time ------------------------------:::::::"<<strTime<<"-----int"<<intTime;
    if(intTime==23 || intTime<5)
    {
        if(intTime==23 ){

            if(intMinute==11)
                DeviceSetting::updateBegin=1;
        }
        else
            DeviceSetting::updateBegin=0;
        //keep update
        return false;
         //return true;
    }
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

void MainFunction::firstHeartBeat()
{
    QString heartBeatUrl;
    newInfo->p_cmdFlag=-1;
    heartBeatUrl= DeviceSetting::serverIP+QString("UBRSSDS&stationCode=%1&deviceStatus=%2").arg(DeviceSetting::stationCode).arg(DeviceSetting::errCode);
    pHttpFun=new HttpFun();
    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(heartBeatUrl);
    qDebug()<<"------------------firstHeartBeat--->heartBeatUrl:"<<heartBeatUrl;
}

void MainFunction::getVersionInfo()
{
    //http://123.207.239.144:10000/YiYangIndex.ashx?ActionKey=GUGBDN&stationCode=G0473

    newInfo->GetUrl(GET_VERSION_INFOMATION);
    pHttpFun=new HttpFun();
    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(newInfo->strUrl);

}

void MainFunction::checkNewVersion()
{
    //reportNewVersionToServer();
    QFile flag_file("/root/newAPP");
    if(flag_file.exists()==true)
    {
        //means current running is the new APP
        //system("rm new");
        system("rm 009_busTest");
        system("cp newAPP 009_busTest");
        //need report to server
        DeviceSetting tmp_Bus;
        tmp_Bus.setConfigTxt("appVersion",DeviceSetting::appVersion);
        tmp_Bus.showDeviceInfo();
        reportNewVersionToServer();
        sleep(5);
        system("reboot");

    }
}

void MainFunction::reportNewVersionToServer()
{
    newInfo->GetUrl(REPORT_VERSION_TO_SERVER);
    pHttpFun=new HttpFun();
    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(newInfo->strUrl);
    qDebug()<<"------------------reportNewVersionToServer"<<newInfo->strUrl;

}




