#ifndef ERRORMSG_H
#define ERRORMSG_H

#include <QObject>

class ErrorMsg : public QObject
{
    Q_OBJECT
public:
    explicit ErrorMsg(QObject *parent = 0);

signals:
   // void signal_getErrorMsg(int code, const QString &strResult);
public slots:
    void slot_getErrorMsg(QtMsgType type, const char *msg);
};

#endif // ERRORMSG_H
