#include "devicesetting.h"
QString DeviceSetting::deviceID=NULL;
QString DeviceSetting::busFd=NULL;
QString DeviceSetting::programVID=NULL;
QString DeviceSetting::serverIP="http://123.207.75.109:10000/YiYangIndex.ashx?ActionKey=";
QString DeviceSetting::usbSet=NULL;
int DeviceSetting::delaySeconds=-1;
int DeviceSetting::maxDelaySeconds=WAIT_COM_RETURN_TIME;
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
                if((i=str.indexOf("forword"))>-1)
                    DeviceSetting::busFd=str.mid(i+8).simplified();
                if((i=str.indexOf("version"))>-1)
                    DeviceSetting::programVID=str.mid(i+8).simplified();
                if((i=str.indexOf("usb"))>-1)
                    DeviceSetting::usbSet=str.mid(i+4).simplified();
                 //qDebug()<<"Line:"<< str<<"i="<<i;
            }
            //qDebug()<<"Debug:Local device ID is"<<DeviceSetting::deviceID;
            //qDebug()<<"Debug:Local bus forword is"<<DeviceSetting::busFd;
            //qDebug()<<"Debug:Local program version is"<<DeviceSetting::programVID;
            fileID.flush();
            fileID.close();
            addActionKey();
            qDebug()<<"Set device finisheds";
        }
    }else{
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
}

void DeviceSetting::initDevice()
{
    SetDevicePosition();
    showDeviceInfo();
    //addActionKey();
}
