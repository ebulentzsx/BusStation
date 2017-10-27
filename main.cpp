#include "busPublic.h"
void outputMessage(QtMsgType type, const char *msg);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    testEnv();
    //qInstallMsgHandler(outputMessage);

    //DeviceSetting busDev;
    DeviceSetting my_Bus;
    my_Bus.initDevice();
    myCOM *my_com;
    my_com=new myCOM();
    secondFunction *watch_function;
    watch_function=new secondFunction();
    QThread com_thread,watch_thread;

    my_com->moveToThread(&com_thread);
      //qDebug()<<"222";
    watch_function->moveToThread(&watch_thread);

    MainFunction *mm=new MainFunction();
    QObject::connect(mm,SIGNAL(signal_init_COM()),my_com,SLOT(slot_init()));
    QObject::connect(mm,SIGNAL(signal_send_COM(QByteArray)),my_com,SLOT(slot_send_COM(QByteArray)));
    QObject::connect(my_com,SIGNAL(signal_getState(QString)),watch_function,SLOT(slot_getState(QString)));
    QObject::connect(mm,SIGNAL(signal_init_watch()),watch_function,SLOT(slot_init_watch()));
    QObject::connect(watch_function,SIGNAL(signal_send_com(QByteArray)),my_com,SLOT(slot_send_COM(QByteArray)));
    qDebug() << QString("main cpp thread id:") << QThread::currentThreadId();
    com_thread.start();
    watch_thread.start();
    //mm->setSys_time();
    //QString strUrl = "http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey=GBRDBDNO&deviceNumber=1314&busForward=1";
    //QString strUrl="http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey=GCSDT";

    //newInfo->SetDevicePosition();
  //  newInfo->GetUrl(1);

   // QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
   // pHttpFun->sendRequest(newInfo->strUrl);

     mm->beginLoop();
  // qDebug()<<"222";
    return a.exec();
}

