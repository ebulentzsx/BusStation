#include "mycom.h"
#include <QTextCodec>
int myCOM::waitCount=0;
bool myCOM::writeState=false;
myCOM::myCOM(QObject *parent) :
     QObject(parent)
{

    getCurrentTime();
    flag_isOpen =false;
    //writeState=true;
    myCom = new Posix_QextSerialPort(DeviceSetting::usbSet,QextSerialBase::Polling);

    readThread=new readComThread();
    //QObject::connect(readThread,SIGNAL(signal_getStateFromCom(QString)), this, SLOT(slot_getStateFromCom(QString)));

    QObject::connect(this,SIGNAL(signal_COM_error(bool)),this,SLOT(slot_re_open_COM(bool)));
}

void myCOM::setCOM()
{

    qDebug()<<"SetCOM OK!";
    myCom->setBaudRate(BAUD115200);          //波特率设置，我们设置为115200
    myCom->setDataBits(DATA_8);            //数据位设置，我们设置为8位数据位
    myCom->setParity(PAR_NONE);           //奇偶校验设置，我们设置为无校验
    myCom->setStopBits(STOP_1);            //停止位设置，我们设置为1位停止位
    myCom->setFlowControl(FLOW_OFF);      //控制流
    myCom->setTimeout(150);
}

int myCOM::openCOM()
{
    qDebug()<<"OpenCOM start---";
    if(flag_isOpen)
    {
        qDebug()<<"Already Open COM!";
        return 0;
    }
    if(myCom->open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        qDebug()<<"OpenCOM OK!";
        //readComThread readThread;

        flag_isOpen = true;
        return 0;
    }
    else
    {
        qDebug()<<"OpenCOM Fail!";

        flag_isOpen = false;
       return -1;
    }
    //qDebug()<<"Open COM end---";
}

int myCOM::recvCOM()
{
    int ret;
    if(flag_isOpen)
    {

        readThread->setCom(myCom);
        readThread->start();
        //qDebug()<<"reading";
        ret = 0;
    }
    else{
        qDebug()<<"COM is Closed!";
        ret=-1;
    }
    return ret;
}
int myCOM::sendCOM(QByteArray buf)
{
    int ret=0;

    QByteArray tmp;
    tmp.append(buf);
    if(flag_isOpen)
    {
        ret =0 ;
         qDebug() << "!!---------myCom->isWritable"<<myCom->isWritable();
        myCom->write(tmp);

        myCom->flush();
        //qDebug() << "----------tmp"<<tmp.toHex();
        //sleep(1);
        //qDebug() << "----------buf"<<buf.toHex();


    }
    else
    {
        qDebug()<<"COM is Closed!";
        emit signal_COM_error(false);
        ret=-1;
    }
      qDebug() << QString("-----------slot in mycom thread id:sendCOM") << QThread::currentThreadId();

    return ret;

}
int myCOM::testCOM()
{
    if(flag_isOpen==false)
    {
        qDebug() <<"COM is Closed!";
        return -1;
    }
    QByteArray buf;
    //QTextCodec* pCodec = QTextCodec::codecForName("GB2312");

    //buf=QByteArray("TestSerial:");
    buf.append(strTime);
    //buf.append(strResult);
    //QString strResult = pCodec->toUnicode(buf);

    //myCom->write(buf);
    //const unsigned char s[5]={0xAB,0xD4,0x1A,0x00,0xBA};//AB D4 1A 00 BA

    //QByteArray buf0;
    //buf0.append(strResult);
    //myCom->write(buf0.toHex().append(0x16));
    myCom->write(buf);
    //myCom->write("68 99 99 99 99 99 99 68 04 17 44 DD 35 34 65 63 64 6A 60 63 6B 60 64 64 53 63 64 6D 63 65 6D 68 66 81 16");
    qDebug() <<"myCOM::testCOM--Send:"<<buf;
    return 0;
    //qDebug() <<"myCOM::testCOM--SendQByteArray:"<<buf0.toHex();

}

void myCOM::getCurrentTime()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    strTime = time.toString("yyyy-MM-dd hh:mm:ss ddd");
}



void myCOM::slot_getStateFromCom(const QString &tmp)
{

}

void myCOM::slot_send_COM(QByteArray buf)
{
   writeState=true;
    myCOM::waitCount --;
    writeLock.lock();

    qDebug() << "Current task counts"<<waitCount;
    sendCOM(buf);
   // sleep(1);
    QByteArray temp;
    temp.clear();
    while(temp.isEmpty()){
    temp= myCom->readAll();
      if(writeState==false)
            break;


    }
if (temp.isEmpty())qDebug()<<"------------------------------------------------------------------------------------------------------------------------------------No display";
else
{
     qDebug()<<"Read OK!Receive:"<<temp.toHex();

      emit signal_getState(temp);
}

    writeLock.unlock();



}

void myCOM::slot_re_open_COM(bool com_state)
{
   closeCOM();
   openCOM();

}

void myCOM::slot_init()
{
    setCOM();
    openCOM();
    //recvCOM();
    qDebug() << QString("slot in myCOM thread id:slot_init") << QThread::currentThreadId();
}


void myCOM::closeCOM()
{

    if(flag_isOpen)
    {
    myCom->close();
    delete myCom;
    myCom = NULL;
    flag_isOpen=false;
    }
    qDebug() <<"myCOM::closeCOM:Closed the COM";
}
