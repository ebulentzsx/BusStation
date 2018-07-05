#include "devicesetting.h"
QString DeviceSetting::deviceID=NULL;
QString DeviceSetting::stationCode=NULL;
QString DeviceSetting::programVID=NULL;
QString DeviceSetting::usbSet=NULL;
QString DeviceSetting::serverIP=NULL;
QString DeviceSetting::hostIP=NULL;
QString DeviceSetting::picVersion=NULL;
QString DeviceSetting::appVersion=NULL;
int DeviceSetting::delaySeconds=-1;
int DeviceSetting::maxDelaySeconds=WAIT_COM_RETURN_TIME;
int DeviceSetting::error_Reboot=0;
int DeviceSetting::com_error_Reboot=0;
int DeviceSetting::serverNoUpdate =0;
QList<QString>  DeviceSetting::actionKey;
DeviceSetting::DeviceSetting()
{


}

void DeviceSetting::showDeviceInfo()
{
    qDebug()<<"Device ID:"<<DeviceSetting::deviceID;
    qDebug()<<"Program version:"<<DeviceSetting::programVID;
    qDebug()<<"Server IP:"<<DeviceSetting::serverIP;
    qDebug()<<"USB setting:"<<DeviceSetting::usbSet;
     qDebug()<<"stationCode:"<<DeviceSetting::stationCode;
    foreach (QString x,DeviceSetting::actionKey) {
        qDebug()<<x;
    }
}

void DeviceSetting::SetDevicePosition()
{
    QFile fileID(ID_FILE_PATH);
    if (fileID.exists()){
        //QMessageBox::information(this,"测试","文件存在");
        if(fileID.open( QIODevice::ReadOnly)==false)
        {
            qDebug()<<"Error:Open config File Failed!";
        }
        else
        {
            qDebug()<<"Success:Open ID File OK!";
            while(!fileID.atEnd()) {
                QByteArray line = fileID.readLine();
                QString str(line);
                int i =-1;
               // deviceID=str.simplified();
               // newInfo->SetDevicePosition(str,"1");

                if((i=str.indexOf("id"))>-1)
                    DeviceSetting::deviceID=str.mid(i+3).simplified();
                if((i=str.indexOf("stationCode"))>-1)
                    DeviceSetting::stationCode=str.mid(i+12).simplified();
                if((i=str.indexOf("version"))>-1)
                    DeviceSetting::programVID=str.mid(i+8).simplified();
                if((i=str.indexOf("usb"))>-1)
                    DeviceSetting::usbSet=str.mid(i+4).simplified();
                if((i=str.indexOf("serverIP"))>-1)
                    DeviceSetting::serverIP=str.mid(i+9).simplified();
                if((i=str.indexOf("hostIP"))>-1)
                    DeviceSetting::hostIP=str.mid(i+7).simplified();
                if((i=str.indexOf("picVersion"))>-1)
                    DeviceSetting::picVersion=str.mid(i+11).simplified();
                 //qDebug()<<"Line:"<< str<<"i="<<i;
            }
            //qDebug()<<"Debug:Local device ID is"<<DeviceSetting::deviceID;
            //qDebug()<<"Debug:Local bus forword is"<<DeviceSetting::busFd;
            //qDebug()<<"Debug:Local program version is"<<DeviceSetting::programVID;
            fileID.flush();
            fileID.close();
            if(DeviceSetting::stationCode.length()<3)
            {
                qDebug()<<"Statition code Error!!Please check!! Then reboot";
                sleep(1000);
            }
            if(DeviceSetting::serverIP.length()<10)
            {
                qDebug()<<"Server IP is NULL !!!";
                DeviceSetting::serverIP="http://http://123.207.239.144:10001/YiYangIndex.ashx?ActionKey=";
                //sleep(1000);
            }
            else
            {
               DeviceSetting::serverIP=QString("http://%1/YiYangIndex.ashx?ActionKey=").arg( DeviceSetting::serverIP);
               qDebug()<<"Server IP get from config file:" <<DeviceSetting::serverIP;
            }
            if(DeviceSetting::hostIP.length()<10)
            {
                qDebug()<<"host  IP is NULL !!!";
                DeviceSetting::hostIP="http://http://123.207.239.144:10001/YiYangIndex.ashx?ActionKey=";
                //sleep(1000);
            }
            else
            {
               DeviceSetting::hostIP=QString("http://%1/YiYangIndex.ashx?ActionKey=").arg( DeviceSetting::hostIP);
               qDebug()<<"host   IP  get from config file:" <<DeviceSetting::hostIP;
            }
            addActionKey();
            qDebug()<<"Set device finished";
        }
    }else
    {
        //QMessageBox::critical(this,"测试","文件不存在");
         qDebug()<<"Error:Can not find ID file!!";
    }

}

void DeviceSetting::addActionKey()
{
    DeviceSetting::actionKey.append("GBRDBDNO");
    DeviceSetting::actionKey.append("GCSDT");
    DeviceSetting::actionKey.append("ADDSELOG");
    DeviceSetting::actionKey.append("ADDSBLOG");
    DeviceSetting::actionKey.append("ADDLEDLOG");
    DeviceSetting::actionKey.append("ADDCTRLLOG");
    DeviceSetting::actionKey.append("ADDBATTERYLOG");
    DeviceSetting::actionKey.append("UDSBNO");
    DeviceSetting::actionKey.append("USMS");
    DeviceSetting::actionKey.append("GUGBDN");
    DeviceSetting::actionKey.append("UUGBDN");
    DeviceSetting::actionKey.append("UBRST");
    DeviceSetting::actionKey.append("ADDSILOG");

}

void DeviceSetting::initDevice()
{
    SetDevicePosition();
    showDeviceInfo();
    //addActionKey();
}

void DeviceSetting::intAppVersion(QString tmpVersion)
{
    DeviceSetting::appVersion=tmpVersion;
}
