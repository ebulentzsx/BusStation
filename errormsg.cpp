#include "errormsg.h"

ErrorMsg::ErrorMsg(QObject *parent) :
    QObject(parent)
{
}

void ErrorMsg::slot_getErrorMsg(QtMsgType type, const char *msg)
{
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
            text = QString("Warning:");
            break;

        case QtCriticalMsg:
            text = QString("Critical:");
            break;

        case QtFatalMsg:
            text = QString("Fatal:");
            abort();
        }
        QString message = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")).arg(text).arg(msg);

        QFile file("log.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << endl;
        file.flush();
        file.close();

        mutex.unlock();
    }
}
