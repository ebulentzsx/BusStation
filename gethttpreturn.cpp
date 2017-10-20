#include "gethttpreturn.h"
//#include "busPublic.h"
 #include <QDateTime>
#define ID_FILE_PATH "./bus.cfg"
GetHttpReturn::GetHttpReturn(QObject *parent) :
    QObject(parent)
{
    SetDevicePosition();
}

void GetHttpReturn::slot_requestFinished(bool bSuccess, const QString &strResult)
{

    if(bSuccess)
    {
        strInfor=strResult;
        switch (p_cmdFlag) {
        case GET_BUS_IFOR:
            GetLines();
            break;
        case GET_SYS_TIME:
            SetSysTime();
            break;
        default:
            break;
        }

    }
    else
    {
        strInfor.clear();
      qWarning("Get from server Failed!!");

    }

}
void GetHttpReturn::SetSysTime()
{

    QString stime=strInfor.mid(strInfor.indexOf("model")+8,19);
    stime.remove(4,1);
    stime.remove(6,1);
    stime="date -s \""+stime+"\"";
    QProcess::execute(stime);
  //QProcess::execute("hwclock --systohc");
  //QProcess::execute("sysnc");
    qDebug()<<"Get time from server SUCCESS"<<stime;

}
void GetHttpReturn::SetDevicePosition()
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
                    deviceID=str.mid(i+3).simplified();
                if((i=str.indexOf("forword"))>-1)
                    busFd=str.mid(i+8).simplified();
                if((i=str.indexOf("version"))>-1)
                    programVID=str.mid(i+8).simplified();
                 qDebug()<<"Line:"<< str<<"i="<<i;
            }
            qDebug()<<"Debug:Local device ID is"<<deviceID;
            qDebug()<<"Debug:Local bus forword is"<<busFd;
            qDebug()<<"Debug:Local program version is"<<programVID;
            fileID.flush();
            fileID.close();
        }
    }else{
        //QMessageBox::critical(this,"测试","文件不存在");
         qDebug()<<"Error:Can not find ID file!!";
    }
    //deviceID=deviceId.simplified();
    //busFd=busForword;
}
void  GetHttpReturn::GetUrl(int cmdFlag)
{
    //QString strUrl = "http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey=GBRDBDNO&deviceNumber=1314&busForward=1";
    strUrl.clear();
    switch (cmdFlag) {
    case GET_BUS_IFOR:
        strUrl= SERVER_IP+QString( "GBRDBDNO&deviceNumber=%1&busForward=%2").arg(deviceID).arg(busFd);
        break;
    case GET_SYS_TIME:
        strUrl= SERVER_IP+QString("GCSDT");
        break;
    default:
        break;
    }
    p_cmdFlag=cmdFlag;
    qDebug()<<"Get Url OK as :"<<strUrl;
}
void  GetHttpReturn::GetLines()
{
    tempList.clear();
    tempList=lineList;
    lineList.clear();
    int j=0;
    QString key_model="model";
    QString key_BCNO="BCNO";
    QString key_BRNO="BRNO";
    QString key_BF="BF";
    QString key_CD="CD";
    QString key_LSC="LSC";
    QString key_Id="Id";
    QString key_Split="\"";
    QString key_GetOne="{";
    QString key_Dot=",";
    QString key_GetOneR="}";
    QString strBCNO;
    QString strBRNO;
    QString strBF;
    int intBF;
    QString strId;
    int intId;
    QString strCD;
    QString strLSC;
    int intLSC;
    QString strModel=strInfor.mid(strInfor.indexOf(key_model)+8);
    j=strModel.indexOf(key_GetOne);
      //qDebug()<<"strModel"<<strModel;
    while(j>-1)
    {
    strModel=strModel.mid(j+1);
    strBCNO=strModel.mid(strModel.indexOf(key_BCNO)+7);
    strBCNO=strBCNO.mid(0,strBCNO.indexOf(key_Split));

    strBRNO=strModel.mid(strModel.indexOf(key_BRNO)+7);
    strBRNO=strBRNO.mid(0,strBRNO.indexOf(key_Split));

    strBF=strModel.mid(strModel.indexOf(key_BF)+4);
    intBF=strBF.mid(0,1).toInt();

    strId=strModel.mid(strModel.indexOf(key_Id)+4);
    intId=strId.mid(0,strId.indexOf(key_Split)-1).toInt();

    strCD=strModel.mid(strModel.indexOf(key_CD)+5);
    strCD=strCD.mid(0,strCD.indexOf(key_Split));

    strLSC=strModel.mid(strModel.indexOf(key_LSC)+5);
    //intLSC=strLSC.mid(0,1).toInt();
    strLSC=strLSC.mid(0,1);
    intLSC=strLSC.toInt();
    BusLine tempLine ;
    /*
    tempLine.setID(intId);
    tempLine.setBCNO(strBCNO);
    tempLine.setBF(intBF);
    tempLine.setBRNO(strBRNO);
    tempLine.setLSC(intLSC);
    tempLine.setCD(strCD);
    tempLine.MSG=strBRNO+"--"+strBCNO+"--"+strLSC+"--"+strCD;
    lineList.append(tempLine);
    */
    tempLine.id=intId;
    tempLine.BCNO=strBCNO;
    tempLine.BusFoward=intBF;
    tempLine.BRNO=strBRNO;
    tempLine.LSC=intLSC;
    tempLine.CD=strCD;
    tempLine.MSG=strBRNO+"--"+strBCNO+"--"+strLSC+"--"+strCD;
    lineList.append(tempLine);
    j=strModel.indexOf(key_GetOne);
   /* qDebug()<<"---------------------------------------------------------------------------";
    qDebug()<<"intId"<<intId;
    qDebug()<<"strBRNO"<<strBRNO;
    qDebug()<<"strBCNO"<<strBCNO;
    qDebug()<<"intBF"<<intBF;
    qDebug()<<"strCD"<<strCD;
    qDebug()<<"intLSC"<<intLSC;
    */

    }
CompareInfo();
   // foreach (BusLine x, lineList) {
        //qDebug()<<"Get from http Success!!--------------------------------------------------";
        //qDebug()<<"intId"<<;
       // qDebug()<<"["<<x.id<<"]"<<x.MSG;
        //qDebug()<<"strBRNO"<<x.BRNO;
       // qDebug()<<"strBCNO"<<x.BCNO;
        //qDebug()<<"intBF"<<x.BusFoward;
       // qDebug()<<"strCD"<<x.CD;
        //qDebug()<<"intLSC"<<x.LSC;
       // qDebug()<<x.id;
    //}

}
void GetHttpReturn::ClearTemp()
{
    lineList.clear();
}

void GetHttpReturn::CompareInfo()
{
    int i=0,x;
    int seed = QDateTime::currentDateTime().time().second();
    srand(seed);
    int test =rand()%15;
    while (i<tempList.count()) {

        x=QString::compare(tempList.at(i).CD,lineList.at(i).CD);
        if(x==0)
        {

       //    if(i==1)
           //  getCOM_buf(lineList.at(i));
            // qDebug()<<lineList.at(i).id<<"NO:UPDATE"<<lineList.at(i).BRNO<<lineList.at(i).CD<<"---------old:"<<tempList.at(i).CD;
            i++;
            continue;
        }
        else
        {
         if(i==2)
           getCOM_buf(lineList.at(i));
             qDebug()<<lineList.at(i).id<<"UPDATE--new:"<<lineList.at(i).BRNO<<lineList.at(i).CD<<"---------old:"<<tempList.at(i).CD;
              i++;
        }
        //msleep(100);

    }
}

void GetHttpReturn::getCOM_buf(BusLine newBus)
{
    QByteArray buf,info;
    buf.clear();
    info.clear();
    uint sum;
    info.append(newBus.BRNO);
    info.append("-");
    info.append(newBus.BCNO);
    info.append("-");
    info.append(newBus.CD);
    //info.append("-");
    //info.append("-");

    //info.append("/n");
    uint len=info.length();
    qDebug()<<"info.length:"<<len;
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
    buf[12]=0x34;
    //waitting for further development
    qDebug()<<"Go to write the COM";
    //info.append(QString::number(newBus.id));
    //info.append(" ");

   // info.append(" ");

    for(int j=0;j<len;++j)
    {
        buf[13+j]=(info[j]+0x33)%256;
    }
    for(int k=0;k<(13+len);++k)
    {
        sum=sum+buf[k];
    }
    buf[13+len]=sum%256;
    buf[13+len+1]=0x16;
    //deal the information of bus which update now
    qDebug()<<"send to com"<<buf.toHex();
    emit signal_writeCom(buf);
    // emit signal_writeCom(0x6899999999999968041B44DD3469656A530903041EECDE07E3E9DEF5EA53E726060D6908F1C216);
}


