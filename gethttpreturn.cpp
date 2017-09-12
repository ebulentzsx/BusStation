#include "gethttpreturn.h"

GetHttpReturn::GetHttpReturn(QObject *parent) :
    QObject(parent)
{
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
void GetHttpReturn::SetDevicePosition(QString deviceId,QString busForword)
{
    deviceID=deviceId;
    busFd=busForword;
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
    int i=0,j=0;
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
      qDebug()<<"strModel"<<strModel;
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
    BusLine tempLine ;
    tempLine.id=intId;
    tempLine.BCNO=strBCNO;
    tempLine.BusFoward=intBF;
    tempLine.BRNO=strBRNO;
    tempLine.LSC=intLSC;
    tempLine.CD=strCD;
    tempLine.MSG=strBRNO+"--"+strBCNO+"--"+strLSC+"--"+strCD+"\n";
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
    foreach (BusLine x, lineList) {
        qDebug()<<"Get from http Success!!--------------------------------------------------";
        qDebug()<<"intId"<<x.id;
        qDebug()<<x.MSG;
        //qDebug()<<"strBRNO"<<x.BRNO;
       // qDebug()<<"strBCNO"<<x.BCNO;
        //qDebug()<<"intBF"<<x.BusFoward;
       // qDebug()<<"strCD"<<x.CD;
        //qDebug()<<"intLSC"<<x.LSC;
       // qDebug()<<x.id;
    }

}
