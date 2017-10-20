#include "secondfunction.h"

secondFunction::secondFunction(QObject *parent) :
    QThread(parent)
{
     timer = new QTimer();
}

void secondFunction::run()
{
    qDebug() << QString("second function thread id:") << QThread::currentThreadId();
    timer->start(5000);
    timer->setSingleShot( false );
}
void secondFunction::slot_getState(const QString &strResult)
{
    QByteArray info;
    info.append(strResult);
    qDebug()<<"second function get state from COM str:"<<strResult;
    qDebug()<<"second function get state from COM info:"<<info;
    qDebug()<<"second function get state from COM"<<info.toHex();
}
