#include "secondfunction.h"

secondFunction::secondFunction(QObject *parent) :
    QObject(parent)
{

     secTimer=0;
     countTimer=-1;

     wtach_timer = new QTimer();
     wetherFinished=true;
     QObject::connect(wtach_timer, SIGNAL(timeout()), this, SLOT(slot_pubulic_timer()));
     qDebug() << QString("second function :slot and signal connected");

}

void secondFunction::beginLoop()
{

    wtach_timer->start(80*1000);
    wtach_timer->setSingleShot(true);

    qDebug() << QString("second function thread id:beginLoop-----") << QThread::currentThreadId();
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


}




void secondFunction::postAll(QByteArray tmp_HardwareInfo)
{
    /*
    BusStationCode：站台编号
    */

    /*-------------------------------------001
    BatteryStatus：蓄电池状态（1是正常，0是异常）
    BatteryOutputVoltage：蓄电池输出电压
    BatteryOutputCurrent：蓄电池输出电流
    BatteryTheoreticalStock：蓄电池电池容量
    BatteryTemperature：蓄电池温度
    BatterySolarChargeStatus：蓄电池太阳能充电状态：1是充电，0是停止
    BatteryCityChargeStatus：市电充电状态：1是充电，0是停止*/
char BatteryStatus,BatterySolarChargeStatus,BatteryCityChargeStatus;
QByteArray BatteryOutputVoltage,BatteryOutputCurrent,BatteryTheoreticalStock,BatteryTemperature;

BatteryStatus=tmp_HardwareInfo[198]-0x33;
BatterySolarChargeStatus=tmp_HardwareInfo[205]-0x33;
BatteryCityChargeStatus=tmp_HardwareInfo[206]-0x33;

BatteryOutputVoltage.append(tmp_HardwareInfo[22]-0x33);
BatteryOutputVoltage.append(tmp_HardwareInfo[23]-0x33);
BatteryOutputVoltage.append('.');
BatteryOutputVoltage.append(tmp_HardwareInfo[24]-0x33);
BatteryOutputVoltage.append(tmp_HardwareInfo[25]-0x33);

BatteryOutputCurrent.append(tmp_HardwareInfo[27]-0x33);
BatteryOutputCurrent.append(tmp_HardwareInfo[28]-0x33);
BatteryOutputCurrent.append('.');
BatteryOutputCurrent.append(tmp_HardwareInfo[29]-0x33);
BatteryOutputCurrent.append(tmp_HardwareInfo[30]-0x33);

BatteryTheoreticalStock.append(tmp_HardwareInfo[32]-0x33);
BatteryTheoreticalStock.append(tmp_HardwareInfo[33]-0x33);
BatteryTheoreticalStock.append('.');
BatteryTheoreticalStock.append(tmp_HardwareInfo[34]-0x33);
BatteryTheoreticalStock.append(tmp_HardwareInfo[35]-0x33);

BatteryTemperature.append("TBD");
    /*-------------------------------------002
    LiBattteryVoltageStatus：锂电池电压状态：1是高压，0是低压
    QdBtnBatteryVoltageStatus：驱动板纽扣电池电压状态：1是正常，0是异常
    ControlPowerStatus：控制板电源开启状态：1是开启，0是关闭
    ControlLcdStatus：控制板LCD运行状态（1是正常，0是异常）
    ControlConnectStatus：控制板电源板与控制板通信状态（1是正常，0是异常）
    QdBtnBatteryVoltage：驱动板纽扣电池电压
    ControlVoltage：控制板电压
    ControlElectricCurrent：控制板电流
    */
char LiBattteryVoltageStatus,QdBtnBatteryVoltageStatus,ControlPowerStatus,ControlLcdStatus,ControlConnectStatus;
QByteArray QdBtnBatteryVoltage,ControlVoltage,ControlElectricCurrent;
LiBattteryVoltageStatus=tmp_HardwareInfo[204]-0x33;
QdBtnBatteryVoltageStatus=tmp_HardwareInfo[195]-0x33;
ControlPowerStatus=tmp_HardwareInfo[207]-0x33;
ControlLcdStatus=tmp_HardwareInfo[187]-0x33;
ControlConnectStatus=tmp_HardwareInfo[185]-0x33;

QdBtnBatteryVoltage.append(tmp_HardwareInfo[71]-0x33);
QdBtnBatteryVoltage.append(tmp_HardwareInfo[72]-0x33);
QdBtnBatteryVoltage.append('.');
QdBtnBatteryVoltage.append(tmp_HardwareInfo[73]-0x33);
QdBtnBatteryVoltage.append(tmp_HardwareInfo[74]-0x33);

ControlVoltage.append(tmp_HardwareInfo[46]-0x33);
ControlVoltage.append(tmp_HardwareInfo[47]-0x33);
ControlVoltage.append('.');
ControlVoltage.append(tmp_HardwareInfo[48]-0x33);
ControlVoltage.append(tmp_HardwareInfo[49]-0x33);

ControlElectricCurrent.append(tmp_HardwareInfo[51]-0x33);
ControlElectricCurrent.append(tmp_HardwareInfo[52]-0x33);
ControlElectricCurrent.append('.');
ControlElectricCurrent.append(tmp_HardwareInfo[53]-0x33);
ControlElectricCurrent.append(tmp_HardwareInfo[54]-0x33);


    /*-------------------------------------003
    LedStatus： LED灯照明状态：1是正常，0是异常
    LedPowerStatus：LED电源开启状态：1是开启，0是关闭
    BodyRedStatus：人体红外运行状态：1是正常，0是异常
    PhotosensitiveCtrlStatus：光感控制器运行状态：1是正常，0是异常
    LedSupplyVoltage：LED供电电压
    LedVoltage：LED灯电压
    LedCurrent： LED灯电流
  */
char LedStatus,LedPowerStatus,BodyRedStatus,PhotosensitiveCtrlStatus;
QByteArray LedSupplyVoltage,LedVoltage,LedCurrent;
LedStatus=tmp_HardwareInfo[200]-0x33;
LedPowerStatus=tmp_HardwareInfo[208]-0x33;
BodyRedStatus=tmp_HardwareInfo[202]-0x33;
PhotosensitiveCtrlStatus=tmp_HardwareInfo[201]-0x33;

LedSupplyVoltage.append(tmp_HardwareInfo[56]-0x33);
LedSupplyVoltage.append(tmp_HardwareInfo[57]-0x33);
LedSupplyVoltage.append('.');
LedSupplyVoltage.append(tmp_HardwareInfo[58]-0x33);
LedSupplyVoltage.append(tmp_HardwareInfo[59]-0x33);

LedVoltage.append(tmp_HardwareInfo[56]-0x33);
LedVoltage.append(tmp_HardwareInfo[57]-0x33);
LedVoltage.append('.');
LedVoltage.append(tmp_HardwareInfo[58]-0x33);
LedVoltage.append(tmp_HardwareInfo[59]-0x33);

LedCurrent.append(tmp_HardwareInfo[61]-0x33);
LedCurrent.append(tmp_HardwareInfo[62]-0x33);
LedCurrent.append('.');
LedCurrent.append(tmp_HardwareInfo[63]-0x33);
LedCurrent.append(tmp_HardwareInfo[64]-0x33);


    /*-------------------------------------004
     * BoxInnerTemp：安全箱箱内温度
    BoxInnerHumidity：安全箱箱内湿度
    BoxShockStatus：安全箱震动传感器状态：1是正常，0是异常
    BoxShockCount：安全箱震动累计次数
    BoxHornStatus：安全箱报警喇叭运行状态：1是正常，0是异常
    BoxHornCount：安全箱喇叭报警次数
    BoxUpDoorStatus：安全箱安全门上门运行状态：1是正常，0是异常
    BoxUpDoorOpenCount：安全箱上门累计打开次数
    BoxDownDoorStatus：安全箱安全门下门运行状态：1是正常，0是异常
    BoxDownDoorOpenCount：安全箱下门累计打开次数
  */
char BoxShockStatus,BoxHornStatus,BoxUpDoorStatus,BoxDownDoorStatus;
QByteArray  BoxInnerTemp,BoxInnerHumidity,BoxShockCount,BoxHornCount, BoxUpDoorOpenCount, BoxDownDoorOpenCount;
BoxShockStatus=tmp_HardwareInfo[193]-0x33;
BoxHornStatus=tmp_HardwareInfo[194]-0x33;
BoxUpDoorStatus=tmp_HardwareInfo[191]-0x33;
BoxDownDoorStatus=tmp_HardwareInfo[192]-0x33;

BoxInnerTemp.append(tmp_HardwareInfo[146]-0x33);
BoxInnerTemp.append(tmp_HardwareInfo[147]-0x33);
BoxInnerTemp.append(tmp_HardwareInfo[148]-0x33);
BoxInnerTemp.append('.');
BoxInnerTemp.append(tmp_HardwareInfo[149]-0x33);
BoxInnerTemp.append(tmp_HardwareInfo[150]-0x33);

BoxInnerHumidity.append(tmp_HardwareInfo[151]-0x33);
BoxInnerHumidity.append(tmp_HardwareInfo[152]-0x33);
BoxInnerHumidity.append('.');
BoxInnerHumidity.append(tmp_HardwareInfo[153]-0x33);
BoxInnerHumidity.append(tmp_HardwareInfo[154]-0x33);

BoxShockCount.append(tmp_HardwareInfo[155]-0x33);
BoxShockCount.append(tmp_HardwareInfo[156]-0x33);
BoxShockCount.append(tmp_HardwareInfo[157]-0x33);
BoxShockCount.append(tmp_HardwareInfo[158]-0x33);
BoxShockCount.append(tmp_HardwareInfo[159]-0x33);
BoxShockCount.append(tmp_HardwareInfo[160]-0x33);

BoxHornCount.append(tmp_HardwareInfo[161]-0x33);
BoxHornCount.append(tmp_HardwareInfo[162]-0x33);
BoxHornCount.append(tmp_HardwareInfo[163]-0x33);
BoxHornCount.append(tmp_HardwareInfo[164]-0x33);
BoxHornCount.append(tmp_HardwareInfo[165]-0x33);
BoxHornCount.append(tmp_HardwareInfo[166]-0x33);

BoxUpDoorOpenCount.append(tmp_HardwareInfo[167]-0x33);
BoxUpDoorOpenCount.append(tmp_HardwareInfo[168]-0x33);
BoxUpDoorOpenCount.append(tmp_HardwareInfo[169]-0x33);
BoxUpDoorOpenCount.append(tmp_HardwareInfo[170]-0x33);
BoxUpDoorOpenCount.append(tmp_HardwareInfo[171]-0x33);
BoxUpDoorOpenCount.append(tmp_HardwareInfo[172]-0x33);


BoxDownDoorOpenCount.append(tmp_HardwareInfo[173]-0x33);
BoxDownDoorOpenCount.append(tmp_HardwareInfo[174]-0x33);
BoxDownDoorOpenCount.append(tmp_HardwareInfo[175]-0x33);
BoxDownDoorOpenCount.append(tmp_HardwareInfo[176]-0x33);
BoxDownDoorOpenCount.append(tmp_HardwareInfo[177]-0x33);
BoxDownDoorOpenCount.append(tmp_HardwareInfo[178]-0x33);


    /*-------------------------------------005
    SolarEnergyStatus：太阳能设备状态
    SolarEnergyOutputVoltage：太阳能输出电压
    SolarEnergyOutputCurrent：太阳能输出电流
    */
char SolarEnergyStatus;
QByteArray SolarEnergyOutputVoltage,SolarEnergyOutputCurrent;
SolarEnergyStatus=tmp_HardwareInfo[196]-0x33;

SolarEnergyOutputVoltage.append(tmp_HardwareInfo[12]-0x33);
SolarEnergyOutputVoltage.append(tmp_HardwareInfo[13]-0x33);
SolarEnergyOutputVoltage.append('.');
SolarEnergyOutputVoltage.append(tmp_HardwareInfo[14]-0x33);
SolarEnergyOutputVoltage.append(tmp_HardwareInfo[15]-0x33);

SolarEnergyOutputCurrent.append(tmp_HardwareInfo[17]-0x33);
SolarEnergyOutputCurrent.append(tmp_HardwareInfo[18]-0x33);
SolarEnergyOutputCurrent.append('.');
SolarEnergyOutputCurrent.append(tmp_HardwareInfo[19]-0x33);
SolarEnergyOutputCurrent.append(tmp_HardwareInfo[20]-0x33);

//-----Gtt url-----
post_url.clear();
if(BatteryStatus==0x00)
    BatteryStatus=0x30;
if(BatterySolarChargeStatus==0x00)
    BatterySolarChargeStatus=0x30;
if(BatteryCityChargeStatus==0x00)
    BatteryCityChargeStatus=0x30;
if(LiBattteryVoltageStatus==0x00)
    LiBattteryVoltageStatus=0x30;
if(QdBtnBatteryVoltageStatus==0x00)
    QdBtnBatteryVoltageStatus=0x30;
if(ControlPowerStatus==0x00)
    ControlPowerStatus=0x30;
if(ControlLcdStatus==0x00)
    ControlLcdStatus=0x30;
if(ControlConnectStatus==0x00)
    ControlConnectStatus=0x30;
if(LedStatus==0x00)
    LedStatus=0x30;
if(LedPowerStatus==0x00)
    LedPowerStatus=0x30;
if(BodyRedStatus==0x00)
    BodyRedStatus=0x30;
if(PhotosensitiveCtrlStatus==0x00)
    PhotosensitiveCtrlStatus=0x30;
if(BoxShockStatus==0x00)
    BoxShockStatus=0x30;
if(BoxHornStatus==0x00)
    BoxHornStatus=0x30;
if(BoxUpDoorStatus==0x00)
    BoxUpDoorStatus=0x30;
if(BoxDownDoorStatus==0x00)
    BoxDownDoorStatus=0x30;
if(SolarEnergyStatus==0x00)
    SolarEnergyStatus=0x30;
//&BusStationCode=Z576-1 &BatteryStatus=1&BatteryOutputVoltage=10&BatteryOutputCurrent=5.0&BatteryTheoreticalStock=10&BatteryTemperature=18&BatterySolarChargeStatus=1&BatteryCityChargeStatus=0&LiBattteryVoltageStatus=0&QdBtnBatteryVoltageStatus=1&QdBtnBatteryVoltage=110&ControlVoltage=220&ControlElectricCurrent=10&ControlPowerStatus=1&ControlLcdStatus=1&ControlConnectStatus=1&LedStatus=1&LedPowerStatus=0&BodyRedStatus=1&PhotosensitiveCtrlStatus=0&LedSupplyVoltage=220&LedVoltage=0.5&LedCurrent=0.6&BoxInnerTemp=18&BoxInnerHumidity=19&BoxShockStatus=1&BoxShockCount=123&BoxHornStatus=1&BoxHornCount=180&BoxUpDoorStatus=0&BoxUpDoorOpenCount=333&BoxDownDoorStatus=1&BoxDownDoorOpenCount=334&SolarEnergyStatus=1&SolarEnergyOutputVoltage=18.5&SolarEnergyOutputCurrent=16.9
post_url= DeviceSetting::hostIP+QString("%1&BusStationCode=%2&BatteryStatus=%3&BatteryOutputVoltage=%4&BatteryOutputCurrent=%5&BatteryTheoreticalStock=%6&BatteryTemperature=%7&BatterySolarChargeStatus=%8&BatteryCityChargeStatus=%9&LiBattteryVoltageStatus=%10&QdBtnBatteryVoltageStatus=%11&QdBtnBatteryVoltage=%12&ControlVoltage=%13&ControlElectricCurrent=%14&ControlPowerStatus=%15&ControlLcdStatus=%16&ControlConnectStatus=%17&LedStatus=%18&LedPowerStatus=%19&BodyRedStatus=%20&PhotosensitiveCtrlStatus=%21&LedSupplyVoltage=%22&LedVoltage=%23&LedCurrent=%24&BoxInnerTemp=%25&BoxInnerHumidity=%26&BoxShockStatus=%27&BoxShockCount=%28&BoxHornStatus=%29&BoxHornCount=%30&BoxUpDoorStatus=%31&BoxUpDoorOpenCount=%32&BoxDownDoorStatus=%33&BoxDownDoorOpenCount=%34&SolarEnergyStatus=%35&SolarEnergyOutputVoltage=%36&SolarEnergyOutputCurrent=%37")
        .arg(DeviceSetting::actionKey.at(5))
        .arg(DeviceSetting::stationCode)
        .arg(BatteryStatus)
        .arg(QString::fromLocal8Bit(BatteryOutputVoltage))
        .arg(QString::fromLocal8Bit(BatteryOutputCurrent))
        .arg(QString::fromLocal8Bit(BatteryTheoreticalStock))
        .arg(QString::fromLocal8Bit(BatteryTemperature))
        .arg(BatterySolarChargeStatus)
        .arg(BatteryCityChargeStatus)
        .arg(LiBattteryVoltageStatus)
        .arg(QdBtnBatteryVoltageStatus)
        .arg(QString::fromLocal8Bit(QdBtnBatteryVoltage))
        .arg(QString::fromLocal8Bit(ControlVoltage))
        .arg(QString::fromLocal8Bit(ControlElectricCurrent))
        .arg(ControlPowerStatus)
        .arg(ControlLcdStatus)
        .arg(ControlConnectStatus)
        .arg(LedStatus)
        .arg(LedPowerStatus)
        .arg(BodyRedStatus)
        .arg(PhotosensitiveCtrlStatus)
        .arg(QString::fromLocal8Bit(LedSupplyVoltage))
        .arg(QString::fromLocal8Bit(LedVoltage))
        .arg(QString::fromLocal8Bit(LedCurrent))
        .arg(QString::fromLocal8Bit(BoxInnerTemp))
        .arg(QString::fromLocal8Bit(BoxInnerHumidity))
        .arg(BoxShockStatus)
        .arg(QString::fromLocal8Bit(BoxShockCount))
        .arg(BoxHornStatus)
        .arg(QString::fromLocal8Bit(BoxHornCount))
        .arg(BoxUpDoorStatus)
        .arg(QString::fromLocal8Bit(BoxUpDoorOpenCount))
        .arg(BoxDownDoorStatus)
        .arg(QString::fromLocal8Bit(BoxDownDoorOpenCount))
        .arg(SolarEnergyStatus)
        .arg(QString::fromLocal8Bit(SolarEnergyOutputVoltage))
        .arg(QString::fromLocal8Bit(SolarEnergyOutputCurrent));
qDebug()<<"post_url:"<<post_url;
qDebug()<<"SolarEnergyOutputVoltage:"<<SolarEnergyOutputVoltage;
qDebug()<<"SolarEnergyOutputCurrent:"<<SolarEnergyOutputCurrent;
updateToServer();
}

void secondFunction::heartBeatToNet()
{
    post_url.clear();
  //  post_url= SERVER_IP+QString("UBRST&stationCode=%1").arg(DeviceSetting::stationCode);
    post_url= DeviceSetting::hostIP+QString("UBRST&stationCode=%1").arg(DeviceSetting::stationCode);
    qDebug()<<"------------------heartBeatToNet]--->post_url:"<<post_url;
    updateToServer();
}
void secondFunction::slot_getState(QByteArray strResult)
{
  qDebug() <<"---------------------secondFunction::slot_getState"<<strResult;

  //  tmp_HardwareInfo.append(strResult);
           // QByteArray::operator [](0);
   // qDebug() << QString("---------------slot in second function thread id:get state") <<info[8]<<QThread::currentThreadId();
     //qDebug() << QString("---------------slot in second function thread id:get state") <<info;
    //qDebug()<<"second function get state from COM"<<tmp_HardwareInfo;
    //qDebug() << QString("slot in mycom thread id:get state") << QThread::currentThreadId();

    // if (( 0x68==info.operator [](0))&&(0x68== info.operator [](7) )&&(0x81==info.operator [](8)))
     if (( 0x68==strResult[0])&&(0x68== strResult[7] )&&(0x81==strResult[8]))
     {
         //tmp_HardwareInfo.clear();
//         tmp_HardwareInfo.append(strResult);
         postAll(strResult);
         qDebug() <<"---------------------Get all status:Return OK"<<strResult;

     }

     if (( 0x68==strResult[0])&&(0x68== strResult[7] )&&(0x84==strResult[8]))
         qDebug() <<"---------------------Control display:Return OK"<<strResult;
}



void secondFunction::slot_init_watch()
{
    //heartBeatToNet();
    beginLoop();
    qDebug() << QString("slot in second function thread id:slot_init_watch") << QThread::currentThreadId();
}

void secondFunction::slot_requestFinished(bool bSuccess, const QString &strResult)
{
    qDebug()<<"secondFunction:::::::slot_requestFinished";
    if(bSuccess)
    {
        wetherFinished=true;
        qDebug()<<"----------------slot_requestFinished"<<post_url;
    }
    else
    {
        qDebug()<<"Second function connect SERVER Error!";

    }
   // wtach_timer->start(600*1000);
}

void secondFunction::slot_pubulic_timer()
{
    if(countTimer==-1)
    {
        //haertbeat
      //  heartBeatToNet();
        get_all_status();
    }

    countTimer++;
    if(countTimer==1)
        get_all_status();

    if(countTimer==2)
    {
        //haertbeat
        heartBeatToNet();
        countTimer=0;
    }
     else
    {
       // qDebug() << QString("slot in second function thread id:slot_send_get_status") << QThread::currentThreadId();
        secTimer=0;
        //update hardware information
    }
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
        qDebug() << QString("slot in second function thread id:slot_pubulic_timer") << QThread::currentThreadId();
        wtach_timer->start(600*1000);
        wtach_timer->setSingleShot(true);
        secTimer=0;
#if GET_ONE_FROM_CQ
#else

#endif
  //  }
}
