#include "busPublic.h"
void outputMessage(QtMsgType type, const char *msg);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
     //system("echo 0 >/dev/watchdog");
    //char buf[2014];
    //QProcess::execute("pppd call wcdma&");
#if    DEBUG_RUN_DESKTOP
#else
    qDebug() <<"<*-------------------------------------------20180117 1056--------------------------------------*>";
    system("pppd call wcdma&");
    sleep(16);
#endif
    testEnv();

    DeviceSetting my_Bus;
    my_Bus.initDevice();

    myCOM *my_com;
    my_com=new myCOM();

    secondFunction *watch_function;
    watch_function=new secondFunction();

    QThread com_thread,watch_thread;

    my_com->moveToThread(&com_thread);
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
    mm->beginLoop();
    return a.exec();
}

