#include "secondfunction.h"

secondFunction::secondFunction(QObject *parent) :
    QObject(parent)
{

     secTimer=0;
     wtach_timer = new QTimer();
     wetherFinished=true;
     QObject::connect(wtach_timer, SIGNAL(timeout()), this, SLOT(slot_pubulic_timer()));
     qDebug() << QString("second function :slot and signal connected");

}

void secondFunction::beginLoop()
{

    wtach_timer->start(60*1000);
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
    wetherFinished=false;
    secHttp=new HttpFun();
    QObject::connect(secHttp,SIGNAL(signal_requestFinished(bool,QString)),this,SLOT(slot_requestFinished(bool,QString)));
    secHttp->sendRequest(post_url);
}

void secondFunction::updateAllToServer()
{
    postSunLog();

}

void secondFunction::postSunLog()
{
   char state;
   QByteArray outputVoltage;
   QByteArray outputCurrent;

   outputVoltage.append(tmp_HardwareInfo[12]-0x33);
   outputVoltage.append(tmp_HardwareInfo[13]-0x33);
   outputVoltage.append('.');
   outputVoltage.append(tmp_HardwareInfo[14]-0x33);
   outputVoltage.append(tmp_HardwareInfo[15]-0x33);

   outputCurrent.append(tmp_HardwareInfo[16]-0x33);
   outputCurrent.append(tmp_HardwareInfo[17]-0x33);
   outputCurrent.append('.');
   outputCurrent.append(tmp_HardwareInfo[18]-0x33);
   outputCurrent.append(tmp_HardwareInfo[19]-0x33);

   state=tmp_HardwareInfo[56]-0x33;

   qDebug()<<"state:"<<state;
   qDebug()<<"outputVoltage:"<<outputVoltage;
   qDebug()<<"outputCurrent:"<<outputCurrent;
   post_url.clear();
   post_url= SERVER_IP+QString("%1&deviceNumber=%2&outputVoltage=%3&outputCurrent=%4&deviceStatus=%5")
           .arg(DeviceSetting::actionKey.at(2))
           .arg(DeviceSetting::deviceID)
           .arg(QString::fromLocal8Bit(outputVoltage))
           .arg(QString::fromLocal8Bit(outputCurrent))
           .arg(state);
   qDebug()<<"post_url:"<<post_url;
   updateToServer();
}

void secondFunction::postBatteryLog()
{
    char deviceStatus,solarChargeStatus,cityChargeStatus,liBattteryVoltageStatus,btnBatteryVStatus;
    QByteArray  outputVoltage,outputCurrent, theoreticalStock,batteryTemperature,tnBatteryV;
    /*
    deviceStatus：蓄电池运行状态（1是正常，0是异常）
    outputVoltage：输出电压（浮点型数字）
    outputCurrent：输出电流（浮点型数字）
    theoreticalStock：电池容量（浮点型数字）
    batteryTemperature：电池温度（浮点型数字）
    solarChargeStatus：太阳能充电状态：1是充电，0是停止
    cityChargeStatus：市电充电状态：1是充电，0是停止
    liBattteryVoltageStatus：锂电池电压状态：1是高压，0是低压
    btnBatteryVStatus：驱动板纽扣电池电压状态：1是正常，0是异常
    btnBatteryV：驱动板纽扣电池电压
    */
    deviceStatus=tmp_HardwareInfo[57]-0x33;
    solarChargeStatus=tmp_HardwareInfo[65]-0x33;
    cityChargeStatus='x';
    liBattteryVoltageStatus='x';
    btnBatteryVStatus='x';
    outputVoltage.append(tmp_HardwareInfo[11]-0x33);
    outputVoltage.append(tmp_HardwareInfo[11]-0x33);
    outputVoltage.append('.');
    outputVoltage.append(tmp_HardwareInfo[11]-0x33);
    outputVoltage.append(tmp_HardwareInfo[11]-0x33);

    outputCurrent.append(tmp_HardwareInfo[11]-0x33);
    outputCurrent.append(tmp_HardwareInfo[11]-0x33);
    outputCurrent.append('.');
    outputCurrent.append(tmp_HardwareInfo[11]-0x33);
    outputCurrent.append(tmp_HardwareInfo[11]-0x33);

    theoreticalStock.append(tmp_HardwareInfo[11]-0x33);
    theoreticalStock.append(tmp_HardwareInfo[11]-0x33);
    theoreticalStock.append('.');
    theoreticalStock.append(tmp_HardwareInfo[11]-0x33);
    theoreticalStock.append(tmp_HardwareInfo[11]-0x33);

    batteryTemperature.append(tmp_HardwareInfo[11]-0x33);
    batteryTemperature.append(tmp_HardwareInfo[11]-0x33);
    batteryTemperature.append(tmp_HardwareInfo[11]-0x33);
    batteryTemperature.append('.');
    batteryTemperature.append(tmp_HardwareInfo[11]-0x33);

    tnBatteryV.append(tmp_HardwareInfo[11]-0x33);
    tnBatteryV.append(tmp_HardwareInfo[11]-0x33);
    tnBatteryV.append('.');
    tnBatteryV.append(tmp_HardwareInfo[11]-0x33);
    tnBatteryV.append(tmp_HardwareInfo[11]-0x33);
}
void secondFunction::slot_getState(const QString &strResult)
{
    QByteArray info;
    info.append(strResult);
  // info.operator [](0);
    tmp_HardwareInfo.append(strResult);
           // QByteArray::operator [](0);
   // qDebug() << QString("---------------slot in second function thread id:get state") <<info[8]<<QThread::currentThreadId();
     //qDebug() << QString("---------------slot in second function thread id:get state") <<info;
    //qDebug()<<"second function get state from COM"<<tmp_HardwareInfo;
    //qDebug() << QString("slot in mycom thread id:get state") << QThread::currentThreadId();

    // if (( 0x68==info.operator [](0))&&(0x68== info.operator [](7) )&&(0x81==info.operator [](8)))
     if (( 0x68==info[0])&&(0x68== info[7] )&&(0x81==info[8]))
     {
         tmp_HardwareInfo.clear();
         tmp_HardwareInfo.append(strResult);
         updateAllToServer();
         qDebug() <<"---------------------Get all status:Return OK"<<info;

     }

     if (( 0x68==info[0])&&(0x68== info[7] )&&(0x84==info[8]))
         qDebug() <<"---------------------Control display:Return OK"<<info;
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
        wetherFinished=true;
    }
    else
    {
        qDebug()<<"Second function get internet Error!";

    }
}

void secondFunction::slot_pubulic_timer()
{
/*
    if(DeviceSetting::delaySeconds>-1)
    {
            DeviceSetting::delaySeconds++;
            qDebug() <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~delaySeconds:"<<DeviceSetting::delaySeconds ;
    }

    secTimer++;
    if(secTimer==SECOND_FUNCTION_INTERVAL)
    {
*/
     //Second funcion do
        qDebug() << QString("slot in second function thread id:slot_send_get_status") << QThread::currentThreadId();
        secTimer=0;
        get_all_status();
  //  }
}
