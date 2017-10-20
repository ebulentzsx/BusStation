#ifndef SECONDFUNCTION_H
#define SECONDFUNCTION_H

#include <QThread>
#include <QtCore>
#include <QTimer>
class secondFunction : public QThread
{
    Q_OBJECT
public:
    QTimer *timer;
    explicit secondFunction(QObject *parent = 0);
    void run();
public slots:
    void slot_getState(const QString &strResult);
};

#endif // SECONDFUNCTION_H
