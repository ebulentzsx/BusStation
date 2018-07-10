#include "gethttpreturn.h"
 #include <QDateTime>

bool GetHttpReturn::deal_one_finish=true;

GetHttpReturn::GetHttpReturn(QObject *parent) :
    QObject(parent)
{
    init_flag=true;
    deal_all_finish=true;
    lastPort=0;
    serverNoUpdateFlag=0;
    refresh_buf.resize(980);
    refresh_buf.fill(0x00);
}

void GetHttpReturn::slot_requestFinished(bool bSuccess, const QString &strResult)
{

    if(bSuccess)
    {
        strInfor.clear();
        DeviceSetting::error_Reboot=0;
        strInfor=strResult;
        switch (p_cmdFlag) {
        case GET_BUS_IFOR:
            GetLines();

            break;
        case GET_SYS_TIME:
            SetSysTime();
            initAll1096();

            break;
        case GET_ONE_FROM_CQ:
             qDebug()<<"Get CQ from server Success!!";
            GetLines();
            break;
        case GET_VERSION_INFOMATION:
            getVersionFromReturn();

            break;
        default:
            break;
        }

    }
    else
    {
        strInfor.clear();
        if(p_cmdFlag==GET_SYS_TIME)
        {
            qDebug()<<"Get TIME!!! from server Failed!!";
            //initAll1096();
            QFile errFile("./errFile");
            if(errFile.exists())
            {   system("rm errFile");
                qDebug()<<"rm errFile Get TIME!!! from server Failed!!";
                sleep(2);
                setPowerOff();

                //powe off and reboot
            }
            else
            {
                //touch errFile
                system("touch errFile");
                qDebug()<<"touch errFile Get TIME!!! from server Failed!!";
                 sleep(2);
                QProcess::execute("reboot");
            }
            return;
        }
        if(p_cmdFlag==GET_BUS_IFOR){
            init_flag=true;
            qDebug()<<"Get bus informatioan from server Failed!!";
            DeviceSetting::error_Reboot++;
            sleep(DeviceSetting::error_Reboot);
            if(DeviceSetting::error_Reboot>4)
            {
                initAll1096();
                qDebug()<<"Get from server Failed!!";
                QProcess::execute("reboot");
            }
            return;
        }
        if(p_cmdFlag==GET_VERSION_INFOMATION)
        {
            //determine whether need update
            qDebug()<<"getVersionFromReturn()";
            return;
        }

    }
     emit signal_startTimer();

}
void GetHttpReturn::SetSysTime()
{


    QString stime=strInfor.mid(strInfor.indexOf("model")+8,19);
     qDebug()<<"Get time from server SUCCESS"<<stime;
    stime.remove(4,1);
    stime.remove(6,1);
    stime.insert(4,".");
    stime.insert(7,".");
    stime.insert(11,"-");
    stime.remove(10,1);
    stime="date -s "+stime;
    // date -s 2011-03-03-13:48:00
    QProcess::execute(stime);
  //QProcess::execute("hwclock --systohc");
  //QProcess::execute("sysnc");
    qDebug()<<"Get time from server SUCCESS"<<stime;

}
void GetHttpReturn::setPowerOff()
{
    QByteArray buf;
    buf.resize(14);
    buf.clear();

  // qDebug()<<"Write the COM info:"<<info;

    buf[0]=0x68;
    for(int i=1;i<7;++i)
        buf[i]=0x99;
    buf[7]=0x68;
    buf[8]=0x04;
    //68 99 99 99 99 99 99 68 04 02 73 DD BC 16
    buf[9]=0x02;
    buf[10]=0x73;
    buf[11]=0xdd;
    buf[12]=0xbc;
    buf[13]=0x16;
    emit signal_writeCom(buf);

    qDebug()<<"setPowerOff:"<<buf.toHex();
    sleep(20);

}

void GetHttpReturn::heartbeatWhenSleep()
{
    QByteArray buf;
    buf.resize(14);
    buf.clear();

  // qDebug()<<"Write the COM info:"<<info;

    buf[0]=0x68;
    for(int i=1;i<7;++i)
    buf[i]=0x99;
    buf[7]=0x68;
    buf[8]=0x04;
    //68 99 99 99 99 99 99 68 04 02 74 DD BD 16
    buf[9]=0x02;
    buf[10]=0x74;
    buf[11]=0xdd;
    buf[12]=0xbd;
    buf[13]=0x16;
    emit signal_writeCom(buf);
    qDebug()<<"heartbeatWhenSleep:"<<buf.toHex();
}

void GetHttpReturn::getVersionFromReturn()
{
    qDebug()<<"getVersionFromReturn begin";
    if(strInfor.length()<200)
    {
        qDebug()<<"getVersionFromReturn error!!";
        return;
    }
    QString key_model="model";
    QString key_AppCurrentVersion="AppCurrentVersion";
    QString key_AppTargetVersion="AppTargetVersion";
    QString key_AppIsUpGradeComplete="AppIsUpGradeComplete";
    QString key_AppUpGradeFile="AppUpGradeFile";

    QString key_PicCurrentVersion="PicCurrentVersion";
    QString key_PicTargetVersion="PicTargetVersion";
    QString key_PicIsUpGradeComplete="PicIsUpGradeComplete";
    QString key_PicupGradeFile="PicupGradeFile";

    QString key_Dot=",";
    QString key_String="\"";

    QString AppCurrentVersion;
    QString AppTargetVersion;
    QString AppIsUpGradeComplete;
    QString AppUpGradeFile;

    QString PicCurrentVersion;
    QString PicTargetVersion;
    QString PicIsUpGradeComplete;
    QString PicupGradeFile;

    QString strModel=strInfor.mid(strInfor.indexOf(key_model)+8);


        AppCurrentVersion=strModel.mid(strModel.indexOf(key_AppCurrentVersion)+19);
        AppCurrentVersion=AppCurrentVersion.mid(0,AppCurrentVersion.indexOf(key_Dot));
        qDebug()<<"AppCurrentVersion"<<AppCurrentVersion;

        AppTargetVersion=strModel.mid(strModel.indexOf(key_AppTargetVersion)+19);
        AppTargetVersion=AppTargetVersion.mid(0,AppTargetVersion.indexOf(key_String));
        qDebug()<<"AppTargetVersion"<<AppTargetVersion;

        AppIsUpGradeComplete=strModel.mid(strModel.indexOf(key_AppIsUpGradeComplete)+22);
        AppIsUpGradeComplete=AppIsUpGradeComplete.mid(0,AppIsUpGradeComplete.indexOf(key_Dot));
        qDebug()<<"AppIsUpGradeComplete"<<AppIsUpGradeComplete;

        AppUpGradeFile=strModel.mid(strModel.indexOf(key_AppUpGradeFile)+17);
        AppUpGradeFile=AppUpGradeFile.mid(0,AppUpGradeFile.indexOf(key_String));
        qDebug()<<"AppUpGradeFile"<<AppUpGradeFile;

        PicCurrentVersion=strModel.mid(strModel.indexOf(key_PicCurrentVersion)+19);
        PicCurrentVersion=PicCurrentVersion.mid(0,PicCurrentVersion.indexOf(key_Dot));
        qDebug()<<"PicCurrentVersion"<<PicCurrentVersion;

        PicTargetVersion=strModel.mid(strModel.indexOf(key_PicTargetVersion)+19);
        PicTargetVersion=PicTargetVersion.mid(0,PicTargetVersion.indexOf(key_String));
        qDebug()<<"PicTargetVersion"<<PicTargetVersion;

        PicIsUpGradeComplete=strModel.mid(strModel.indexOf(key_PicIsUpGradeComplete)+22);
        PicIsUpGradeComplete=PicIsUpGradeComplete.mid(0,PicIsUpGradeComplete.indexOf(key_Dot));
        qDebug()<<"PicIsUpGradeComplete"<<PicIsUpGradeComplete;

        PicupGradeFile=strModel.mid(strModel.indexOf(key_PicupGradeFile)+17);
        PicupGradeFile=PicupGradeFile.mid(0,PicupGradeFile.indexOf(key_String));
        qDebug()<<"PicupGradeFile"<<PicupGradeFile;

    if(0==QString::compare("false",AppIsUpGradeComplete,Qt::CaseInsensitive))
    {//download last version
        if(0!=QString::compare(DeviceSetting::appVersion,AppTargetVersion,Qt::CaseInsensitive))
        {
         QByteArray downloadString="curl -m 5 -O http://www.cqfog.com.cn:10000/";
         downloadString.append(AppUpGradeFile);
         system(downloadString);
         qDebug()<<"-----downloadString in shell:"<<downloadString;
         //replace the new app
         QString tmpNewAPPDir;
         tmpNewAPPDir="/root/"+AppTargetVersion;
          qDebug()<<"tmpNewAPPDir"<<tmpNewAPPDir;
         QFile app_file(tmpNewAPPDir);
         if (app_file.exists()){
             //QMessageBox::information(this,"测试","文件存在");
             qDebug()<<"The new APP file have been download sucesfully!";
             qDebug()<<"TAppTargetVersion.length()"<<AppTargetVersion.length()<<"AppTargetVersion[0]"<<AppTargetVersion[0];
             if( (AppTargetVersion.length()==11) && (AppTargetVersion[0]=='D'))
             {
                QByteArray copyCommond="copy ";
                copyCommond.append(AppTargetVersion);
                copyCommond.append(" 009_busTest_new");
                system(copyCommond);
                qDebug()<<"copyCommond"<<copyCommond;
             }
         }else
         {
             //QMessageBox::critical(this,"测试","文件不存在");
             qDebug()<<"The new APP file have been download failed!";

         }



        }

    }
    if(0==QString::compare("false",PicIsUpGradeComplete,Qt::CaseInsensitive))
        {//download last picture
        //replace the new bmp
        QByteArray downloadString="curl -m 5 -O http://www.cqfog.com.cn:10000/";
        downloadString.append(PicupGradeFile);
        system(downloadString);
        qDebug()<<"-----downloadString in shell:"<<downloadString;
        QString tmpNewBMPDir;
        tmpNewBMPDir="/root/"+PicTargetVersion+".bmp";
        qDebug()<<"tmpNewBMPDir"<<tmpNewBMPDir;
        QFile bmp_file(tmpNewBMPDir);
        if (bmp_file.exists()){
        //QMessageBox::information(this,"测试","文件存在");
            qDebug()<<"The new BMP file have been download sucesfully!";
            qDebug()<<"TAppTargetVersion.length()"<<PicTargetVersion.length()<<"AppTargetVersion[0]"<<PicTargetVersion[0];
            if( (PicTargetVersion.length()==11) && (PicTargetVersion[0]=='P'))
                {
                    QByteArray copyPicCommond="copy ";
                    copyPicCommond.append(PicTargetVersion);
                    copyPicCommond.append(".bmp 1063.bmp");
                    system(copyPicCommond);
                    qDebug()<<"copyPicCommond"<<copyPicCommond;
                }
            }else
            {
                //QMessageBox::critical(this,"测试","文件不存在");
                qDebug()<<"The new APP file have been download failed!";

            }
       }
    updateAPP();
    qDebug()<<"getVersionFromReturn ending";
}

void GetHttpReturn::updateAPP()
{//Compaer and copy and set new app

    qDebug()<<"updateAPP begin"<<DeviceSetting::appVersion;
    system("ls -l");

}
void  GetHttpReturn::GetUrl(int cmdFlag)
//http://123.207.75.109:10001/YiYangIndex.ashx?ActionKey=GCQSD&stationCode=B170-1
{
    //QString strUrl = "http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey=GBRDBDNO&deviceNumber=1314&busForward=1";
    strUrl.clear();
    switch (cmdFlag) {
    case GET_BUS_IFOR:
        //strUrl= SERVER_IP+QString( "GBRDBDNO&deviceNumber=%1&busForward=%2").arg(deviceID).arg(busFd);
        strUrl= DeviceSetting::serverIP+QString( "GCQSD&stationCode=%1").arg(DeviceSetting::stationCode);
        break;
    case GET_SYS_TIME:
        strUrl= DeviceSetting::serverIP+QString("GCSDT");
        break;
    case GET_ONE_FROM_CQ:
        strUrl= DeviceSetting::serverIP+QString("GCQD");
        break;
    case SERVER_NO_UPDATE:
        strUrl= DeviceSetting::serverIP+QString("UBRSSUS&status=%1").arg(1-DeviceSetting::serverNoUpdate);
        break;
    case GET_VERSION_INFOMATION:
        strUrl= DeviceSetting::hostIP+QString("GUGBDN&stationCode=%1").arg(DeviceSetting::stationCode);
        break;
    default:
        break;
    }
    p_cmdFlag=cmdFlag;
    qDebug()<<"Get Url OK as :"<<strUrl;
}
void  GetHttpReturn::GetLines()
{
#if DEBUG_GET_ONE_STATION_FROM_CQ
    qDebug()<<"GetLines begin";
    if(strInfor.length()<200)
    {
        qDebug()<<"GetLines error!!";
        return;
    }
    tempList.clear();
    tempList=lineList;
    lineList.clear();
    int j=0;
    QString key_model="model";
    QString key_BCNO="BCNO";
    QString key_VNO="VNO";
    QString key_STime="STime";
    QString key_BRNO="BRNO";
    QString key_BF="BF";
    QString key_CD="CD";
    QString key_LSC="LSC";
    QString key_ESN="ESN";
    QString key_Id="Id";
    QString key_Split="\"";
    QString key_GetOne="{";
    QString key_Dot=",";
    QString key_GetOneR="}";
    QString strBCNO;
    QString strVNO;
    QString strSTime;
    QString strBRNO;
    QString strBF;
    QString strESN;
    int intBF;
    QString strId;
    int intId;
    QString strCD;
    QString strLSC;
    int intLSC;
    QString strModel=strInfor.mid(strInfor.indexOf(key_model)+8);
    j=strModel.indexOf(key_GetOne);
     // qDebug()<<"strModel"<<strModel;
    while(j>-1)
    {
        strModel=strModel.mid(j+1);

        strBCNO=strModel.mid(strModel.indexOf(key_BCNO)+7);
        strBCNO=strBCNO.mid(0,strBCNO.indexOf(key_Split));

        strVNO=strModel.mid(strModel.indexOf(key_VNO)+6);
        strVNO=strVNO.mid(0,strVNO.indexOf(key_Split));

        strESN=strModel.mid(strModel.indexOf(key_ESN)+6);
        strESN=strESN.mid(0,strESN.indexOf(key_Split));

        strSTime=strModel.mid(strModel.indexOf(key_STime)+8);
        strSTime=strSTime.mid(0,strSTime.indexOf(key_Split));

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
        strLSC=strLSC.mid(0,strLSC.indexOf(key_Dot));
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
#if DEBUG_4_DISPLAY
         tempLine.id=intId%3+2;
         tempLine.numPort=1;
#else
        if(intId>18)
            intId=intId%18;
        if(intId%2==1)
        {
            intId=(intId+1)/2+1;
        }
        else if(intId%2==0)
        {
            intId=(intId)/2+11;
        }
        tempLine.id=intId;
        if(intId<9)
            tempLine.numPort=1;
        else if(intId>10 && intId<19)
            tempLine.numPort=2;
        else if(intId==9 || intId==10 || intId==19 || intId==20)
            tempLine.numPort=3;
#endif
        tempLine.BCNO=strBCNO;
        tempLine.BusFoward=intBF;
        tempLine.BRNO=strBRNO;
        tempLine.LSC=intLSC;
        tempLine.CD=strCD;
        tempLine.BCSta=strVNO;
        tempLine.STime=strSTime;
        tempLine.ESN=strESN;
        tempLine.VNO=strVNO;
        tempLine.MSG=strBRNO+"--"+strBCNO+"--"+strLSC+"--"+strCD;
        lineList.append(tempLine);
        j=strModel.indexOf(key_GetOne);
    }
#if NEW_SHOW_1096
    showAll_1096();
#else
    CompareInfo();
#endif
     qDebug()<<"GetLines end";
#else
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

    if(intId>18)
        intId=intId-18;
    if(intId%2==1)
    {
        intId=(intId+1)/2+1;
    }
    else if(intId%2==0)
    {
        intId=(intId)/2+11;
    }
    tempLine.id=intId;
    if(intId<9)
        tempLine.numPort=1;
    else if(intId>10 && intId<19)
        tempLine.numPort=2;
    else if(intId==9 || intId==10 || intId==19 || intId==20)
        tempLine.numPort=3;
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
#endif

}
void GetHttpReturn::ClearTemp()
{
    lineList.clear();
}

void GetHttpReturn::CompareInfo()
{
    if(tempList.isEmpty() && init_flag)
    {
        deal_all_finish=false;
        foreach (BusLine x, lineList) {
             getCOM_buf(x);
        }
        deal_all_finish=true;
        init_flag=false;
        return;
    }
    int i=0,x;
    int new_count,tmp_count;
    new_count = lineList.count();
    tmp_count = tempList.count();
    if(new_count<tmp_count)
    {
        qDebug()<<"Refresh from server ERROR!!";
        return;
    }
    deal_all_finish=false;
    while (i<tmp_count) {

        x=QString::compare(tempList.at(i).CD,lineList.at(i).CD);
        if(x==0)
        {

       //    if(i==1)
           //  getCOM_buf(lineList.at(i));
             qDebug()<<lineList.at(i).id<<"NO:UPDATE"<<lineList.at(i).BRNO<<lineList.at(i).CD<<"---------old:"<<tempList.at(i).CD;
#if DEBUT_WITHOUT_COM
#else
             //getCOM_buf(lineList.at(i));//refresh all
#endif
            i++;
            continue;
        }
        else
        {
            qDebug()<<lineList.at(i).id<<"UPDATE--new:"<<lineList.at(i).BRNO<<lineList.at(i).CD<<"---------old:"<<tempList.at(i).CD;
         //if(i<8)
#if DEBUT_WITHOUT_COM
#else
             getCOM_buf(lineList.at(i));//refresh all
#endif


              i++;
        }
        //msleep(100);

    }
    deal_all_finish=true;
}

void GetHttpReturn::myDelay()
{
    DeviceSetting::delaySeconds=0;

    while(1)
    {
        if(DeviceSetting::delaySeconds==-1)
            break;
    }
    sleep(2);
        myCOM::writeState=false;
}

void GetHttpReturn::showAll_1096()
{
    int updateFlag=0;
    //refresh_buf.fill(0x00);
    int i=0,x;
    int new_count,tmp_count;
    u_int8_t disNum;
    new_count = lineList.count();
    tmp_count = tempList.count();
    if((new_count!=tmp_count) && (init_flag==false))
    {
        qDebug()<<"Refresh from server ERROR!!";
        init_flag=true;
        return;
    }
    int reDisplay;
    reDisplay=new_count-17;
    deal_all_finish=false;
    while (i<new_count) {
        //When lines' number is more than 18,it should be deal again
        if( i>17 && (i%18==0))
        {
            sendAllToCom();
        }
        disNum=lineList.at(i).id;
        //---------------------------------first time
        if(init_flag)
        {
            refresh_buf[12+(lineList.at(i).id-1)*49]=disNum^0x80;
            dealOneLine(lineList.at(i),13+(lineList.at(i).id-1)*49);
            i++;
            continue;
        }
        //--------------------------------
 #if DEBUG_SHOW_DISTANCE
        x=QString::compare(tempList.at(i).CD,lineList.at(i).CD);
#else
        x=tempList.at(i).LSC-lineList.at(i).LSC;
#endif
        if((x==0))
        {

                if(i%18<reDisplay)
                {
                    refresh_buf[12+(lineList.at(i).id-1)*49]=disNum^0x80;
                    dealOneLine(lineList.at(i),13+(lineList.at(i).id-1)*49);
                }
                else
                    refresh_buf[12+(lineList.at(i).id-1)*49]=disNum^0x00;
            //dealOneLine(lineList.at(i),13+(lineList.at(i).id-1)*49);
            //qDebug()<<"bufPosition"<<bufPosition;
            i++;
            continue;
        }
        else
        {
            updateFlag++;
            refresh_buf[12+(lineList.at(i).id-1)*49]=disNum^0x80;
            dealOneLine(lineList.at(i),13+(lineList.at(i).id-1)*49);
            //qDebug()<<"bufPosition"<<bufPosition;
            i++;

        }

    }
    sendAllToCom();
    init_flag=false;
if(updateFlag<1)
{
    serverNoUpdateFlag++;
    if(serverNoUpdateFlag==30)
       {
        serverNoUpdateFlag=29;
        if( DeviceSetting::serverNoUpdate==0)
                DeviceSetting::serverNoUpdate=1; //report to server!!
        qDebug()<<"Server NOUPDATE found!";
       }
}
else
{
    if(DeviceSetting::serverNoUpdate==2)
    {
        DeviceSetting::serverNoUpdate=3;
        qDebug()<<"Server NOUPDATE rescue!";
    }
    else
        DeviceSetting::serverNoUpdate=0;
    serverNoUpdateFlag=0;
}

qDebug()<<"updateFlag--"<<updateFlag<<"serverNoUpdateFlag--"<<serverNoUpdateFlag<<"DeviceSetting::serverNoUpdate--"<<DeviceSetting::serverNoUpdate;
qDebug()<<"showAll_1096 end!!";
    deal_all_finish=true;

}

void GetHttpReturn::sendAllToCom()
{
    u_int8_t sum=0;
    refresh_buf[0]=0x68;
    for(int i=1;i<7;++i)
        refresh_buf[i]=0x99;
    refresh_buf[7]=0x68;
    refresh_buf[8]=0x04;
    refresh_buf[9]=0xff;
    refresh_buf[10]=0x66;
    refresh_buf[11]=0xdd;
    for(int j=0;j<980;++j)
    {
        refresh_buf[12+j]=(refresh_buf[12+j]+0x33)%256;
    }

    for(int k=0;k<992;++k)
    {
        sum=(sum+refresh_buf[k])%256;
    }
    refresh_buf[992]=sum;
    refresh_buf[993]=0x16;
    emit signal_writeCom(refresh_buf);//send to COM
#if DEBUG_PRINT
    qDebug()<<"----------------sendAllToCOM:"<<refresh_buf;
#endif
    myDelay();
    refresh_buf.fill(0x00);

}

void GetHttpReturn::initAll1096()
{
    refresh_buf.fill(0x00);
    QByteArray info;

    info.clear();


    info.append(0xcf);
    info.append(0xdf);//xian--
    info.append(" ");
    info.append(0xc2);
    info.append(0xb7);//lu--
    info.append("  ");

#if DEBUG_SHOW_DISTANCE
    //info.append(newBus.BCNO);
    info.append(0xca);
    info.append(0xa3);//sheng
    info.append(0xd3);
    info.append(0xe0);//yu--
    info.append(0xd5);
    info.append(0xbe);//zhan
    info.append(" ");
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
    //info.append(" ");
    info.append(0xb5);
    info.append(0xe3);
    //info.append(" ");
    info.append(0xd5);
    info.append(0xbe);//zhan
    info.append("    ");
    info.append(0xca);
    info.append(0xa3);//sheng
    info.append(0xd3);
    info.append(0xe0);//yu--
    info.append(0xd5);
    info.append(0xbe);//zhan
#endif
    refresh_buf[12]=0x01^0x80;
    refresh_buf.replace(13,info.length(),info);
    refresh_buf[502]=0x0b^0x80;
    refresh_buf.replace(503,info.length(),info);
    for(u_int8_t i =0;i<20;++i)
    {
        if(i==10||i==0)
            continue;
        if(i==19)
        {
         //   refresh_buf.replace(13+i*49,11,"  Testing...");
          //  refresh_buf[12+i*49]=(i+1)^0x80;
        //     continue;
        }
        refresh_buf[12+i*49]=(i+1)^0x80;
        refresh_buf.replace(13+i*49,5,"     ");
    }
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

     sendAllToCom();


}

void GetHttpReturn::dealOneLine(BusLine newBus,int position)
{

    QByteArray info;

   // int currentDistance;
    //currentDistance=newBus.CD.length();

    info.clear();
  //  u_int8_t sum;
#if DEBUG_SHOW_DISTANCE
    info.append(" ");
#else
#endif
    //info.resize(35);
    //info.replace(0,3,newBus.BRNO);
    //info.replace(3,1,"-");
    info.append(newBus.BRNO);
    if(newBus.BRNO.length()==3)
        info.append(" ");//xianlu
#if DEBUG_SHOW_DISTANCE
#else
    int tmepESN_Length=0;
    tmepESN_Length=newBus.ESN.length();
    if(tmepESN_Length<4)
        info.append("NULL");
    else if(tmepESN_Length<5)
    {
        info.append(" ");
        info.append(newBus.ESN);
        info.append("      ");
    }
    else if(tmepESN_Length<7)
    {
        info.append(" ");
        info.append(newBus.ESN);
        info.append("     ");
    }
    else if(newBus.ESN.length()<9)
    {
        info.append(" ");
        info.append(newBus.ESN);
        info.append("    ");
    }
    else if(newBus.ESN.length()<11)
    {
        info.append(" ");
        info.append(newBus.ESN);
        info.append("   ");
    }
    else if(newBus.ESN.length()<13)
    {
        info.append(" ");
        info.append(newBus.ESN);
        info.append("  ");
    }
    else  if(newBus.ESN.length()<15)
    {
        info.append(" ");
        info.append(newBus.ESN);
        info.append(" ");
    }
    else  if(newBus.ESN.length()<17)
    {
        info.append(" ");
        info.append(newBus.ESN);
        //info.append(" ");
    }
    else if(newBus.ESN.length()<19)
    {
        info.append(" ");
        newBus.ESN=newBus.ESN.left(16);
        info.append(newBus.ESN);
    }
   // info.append(" ");//zhong dian zhan
#endif
#if  DEBUG_SHOW_0_ZHAN
    if(newBus.LSC>0)
    {
       //newBus.LSC=newBus.LSC+9;
        if(newBus.LSC<10)
            info.append(" ");

        info.append(QString::number(newBus.LSC,10));
       // qDebug()<<"----------------newBus.LSC:"<<newBus.LSC<<"---INFO:"<<info;
        //QString::number();
        info.append(0xd5);
        info.append(0xbe);
        info.append(" ");
    }
    else
    {
        if(newBus.VNO.length()==6)
        {
            info.append(newBus.VNO);
           //  qDebug()<<"----------------newBus.VNO.toHex:"<<newBus.LSC;
             //B4FD`
            info.append(" ");
        }
        else
        {
            info.append(0xd5);
            info.append(0xfd);
            info.append(0xd4);
            info.append(0xda);
            info.append(0xbd);
            info.append(0xf8);
            info.append(0xd5);
            info.append(0xbe);
        }

    }
#else


    if(currentDistance<2 )
    {
#if DEBUG_SHOW_DISTANCE
        info.append("    ");
        info.append(newBus.BCSta);
#else

        if(newBus.BCSta.length()==6)
        {
            info.append(" ");

            info.append(0xb4);
            info.append(0xfd);
            info.append(0xb7);
            info.append(0xa2);
            info.append(0xb3);
            info.append(0xb5);
        }
        //B4FD DAI
        //B7A2 FA
        //B3B5 CHE
            //BCB4 JI
            //BDAB JIANG
            //B5BD DAO
            //D5BE zhan
            //info.append(" ");
            /*
            info.append(0xbc);
            info.append(0xb4);
            info.append(0xbd);
            info.append(0xab);
            info.append(0xb5);
            info.append(0xbd);
            info.append(0xd5);
            info.append(0xbe);
*/


#endif
        /*dai fa che

        //info.append("-");
       // info.append("-");
       */

    }
    if(currentDistance<7)
    {
#if DEBUG_SHOW_DISTANCE
        info.append(0xca);
        info.append(0xa3);
        info.append(QString::number(newBus.LSC));
        info.append(0xd5);
        info.append(0xbe);
        info.append(" ");
        info.append(newBus.CD);
        info.append(0xc3);
        info.append(0xd7);//mi
#else
        if(newBus.BCSta.length()!=6)
        {
            if(newBus.LSC>9)
                 info.append(" ");
            else
                 info.append("  ");
            info.append(QString::number(newBus.LSC));
            info.append(0xd5);
            info.append(0xbe);
        }
#endif
    }
    else
    {
#if DEBUG_SHOW_DISTANCE
        info.append(0xca);
        info.append(0xa3);
        info.append(QString::number(newBus.LSC));
        info.append(0xd5);
        info.append(0xbe);
        info.append(" ");
        info.append(newBus.CD);
#else

        info.append(newBus.CD);
#endif
    }
#endif
    uint len=info.length();
     qDebug()<<"----------------dealOneLine:"<<info;
    refresh_buf.replace(position,len,info);
     qDebug()<<"----------------refresh_buf.replace(i,len,info):i--len"<<position<<"--"<<len;
}

void GetHttpReturn::getCOM_buf(BusLine newBus)
{
#if DEBUG_GET_ONE_STATION_FROM_CQ
    QByteArray buf,info;
    int currentDistance;
    currentDistance=newBus.CD.length();
    buf.clear();
    info.clear();
    u_int8_t sum;
    //info.append(" ");
    info.append(newBus.BRNO);
    info.append("-");
#if DEBUG_SHOW_DISTANCE
#else
    if(newBus.ESN.length()<4)
        info.append("NULL");
    else
        info.append(newBus.ESN);
    info.append("-");
#endif
    //info.append("-");
    //info.append(newBus.BCNO);
   // if(QString::number(newBus.LSC)==0)
    if(currentDistance<3)
    {
#if DEBUG_SHOW_DISTANCE
        info.append(" - - -");
        info.append(newBus.BCSta);
#else
         info.append(newBus.BCSta);
#endif
        /*dai fa che

        //info.append("-");
       // info.append("-");
       */

    }
    else if(currentDistance<7)
    {
#if DEBUG_SHOW_DISTANCE
        info.append(0xca);
        info.append(0xa3);
        info.append(QString::number(newBus.LSC));
        info.append(0xd5);
        info.append(0xbe);
        info.append("-");
        info.append(newBus.CD);
        info.append(0xc3);
        info.append(0xd7);//mi
#else

        info.append(QString::number(newBus.LSC));
        info.append(0xd5);
        info.append(0xbe);
#endif
        /*sheng ji zhan

        */

    }
    else
    {
#if DEBUG_SHOW_DISTANCE
        info.append(0xca);
        info.append(0xa3);
        info.append(QString::number(newBus.LSC));
        info.append(0xd5);
        info.append(0xbe);
        info.append("-");
        info.append(newBus.CD);
#else

         info.append(newBus.CD);
#endif
        /* jijiang daozhan

        */

        //info.append("-");

    }



    //D5BE zhan
    //CAA3 sheng

    //info.right()
    //info.append("-");
    //info.append("-");

    //info.append("/n");
    int len=info.length();
    //qDebug()<<"info.length:"<<len;
    //int len=info.count();
   qDebug()<<"----------------------------------------------------------------------------------------------------------Write the COM info:"<<newBus.id<<"---"<<info;
   // int len=newBus.BRNO.length()+newBus.BCNO.length()+newBus.CD.length();
    buf[0]=0x68;
    for(int i=1;i<7;++i)
        buf[i]=0x99;
    buf[7]=0x68;
    buf[8]=0x04;
    buf[9]=1+2+len;
    buf[10]=0x66;
    buf[11]=0xdd;


   // qDebug()<<"----------------------------------------------------------------------------------------------------------Write the COM info:"<<numSecreen<<"---"<<info;

    buf[12]=newBus.id+0x33;
    /*
    if(newBus.id<10)
      buf[12]=newBus.id+0x01+0x33;
    else if(newBus.id<20)
        buf[12]=newBus.id+0x02+0x33;
    //number of screen
    */

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
    qDebug()<<"GetHttpReturn--send to com:getCOM_buf"<<buf.toHex();

    if(lastPort==newBus.numPort)
    {
         qDebug()<<"No change COM port sleep 1s";
       sleep(1);
    }
    lastPort=newBus.numPort;

    emit signal_writeCom(buf);//send to COM

    myCOM::waitCount ++;


//{

    myDelay();
#else
    QByteArray buf,info;
    buf.clear();
    info.clear();
    u_int8_t sum;
    info.append(" ");
    info.append(newBus.BRNO);
    info.append("-");
    //info.append(newBus.BCNO);
    info.append(0xca);
    info.append(0xa3);

    info.append(QString::number(newBus.LSC));
    info.append(0xd5);
    info.append(0xbe);
    info.append("-");
    info.append(newBus.CD);
    if(newBus.CD.length()<7)
    {
    info.append(0xc3);
    info.append(0xd7);//m
    }
    //D5BE zhan
    //CAA3 sheng

    //info.right()
    //info.append("-");
    //info.append("-");

    //info.append("/n");
    uint len=info.length();
    //qDebug()<<"info.length:"<<len;
    //int len=info.count();
   qDebug()<<"----------------------------------------------------------------------------------------------------------Write the COM info:"<<newBus.id<<"---"<<info;
   // int len=newBus.BRNO.length()+newBus.BCNO.length()+newBus.CD.length();
    buf[0]=0x68;
    for(int i=1;i<7;++i)
        buf[i]=0x99;
    buf[7]=0x68;
    buf[8]=0x04;
    buf[9]=1+2+len;
    buf[10]=0x66;
    buf[11]=0xdd;


   // qDebug()<<"----------------------------------------------------------------------------------------------------------Write the COM info:"<<numSecreen<<"---"<<info;

    buf[12]=newBus.id+0x33;
    /*
    if(newBus.id<10)
      buf[12]=newBus.id+0x01+0x33;
    else if(newBus.id<20)
        buf[12]=newBus.id+0x02+0x33;
    //number of screen
    */

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

    if(lastPort==newBus.numPort)
    {
         qDebug()<<"No change COM port sleep 1s";
       sleep(1);
    }
    lastPort=newBus.numPort;

    emit signal_writeCom(buf);//send to COM
    myCOM::waitCount ++;


//{
    myDelay();
#endif
   }






