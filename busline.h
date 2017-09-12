#ifndef BUSLINE_H
#define BUSLINE_H
#include <QString>
class BusLine
{
public:
    BusLine();
    int id;
    int BusFoward;  // back or forward
    int LSC;  //left stations  2
    QString BCNO; //BG4532
    QString BRNO;//line name  325
    QString CD;//distance  1800m
    QString MSG;
    void GetOneLineFromString(QString strModel);
};

#endif // BUSLINE_H
