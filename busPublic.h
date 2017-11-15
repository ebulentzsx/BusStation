#ifndef BUSPUBLIC_H
#define BUSPUBLIC_H
#include <QCoreApplication>
#include <qdebug.h>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTimer>
 #include <QTcpSocket>
#include "mycom.h"
#include "mainfunction.h"
#include "devicesetting.h"
#include "pubulicDefine.h"


/*
qDebug("This is a debug message");
qWarning("This is a warning message");
*/
static bool IPLive(QString ip, int port, int timeout=200)
{
         QTcpSocket tcpClient;
         tcpClient.abort();
         tcpClient.connectToHost(ip, port);
         //100毫秒没有连接上则判断不在线
         return tcpClient.waitForConnected(timeout);
}

void outputMessage(QtMsgType type, const char *msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Error:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
        abort();
    }
    QString message = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")).arg(text).arg(msg);

    QFile file(LOG_FILE_PATH);
    if(file.open( QIODevice::WriteOnly | QIODevice::Append)==false)
    {
    }
    else
    {
        QTextStream text_stream(&file);
        text_stream << message << endl;
        file.flush();
        file.close();
    }

    mutex.unlock();
}

void testEnv()
{

//test file
  QFile file(LOG_FILE_PATH);
  if (file.exists()){
      //QMessageBox::information(this,"测试","文件存在");
  }else{
      //QMessageBox::critical(this,"测试","文件不存在");

  }
//test internet
   if(IPLive("115.239.211.112", 80))
       qDebug()<<"Test internet OK!!";
   else
        {
       qDebug()<<"Test internet Failed!!";
        QProcess::execute("reboot");
   }
//Get device information


}

#endif // BUSPUBLIC_H
