#ifndef BUSLINE_H
#define BUSLINE_H
#include <QString>

class BusLine
{

public:
    BusLine();
    int flag;
    int id;
    int BusFoward,BusFoward0;  // back or forward
    int LSC;  //left stations  2
    QString BCNO; //BG4532
    QString BRNO;//line name  325
    QString CD;//distance  1800m
    QString MSG;
    void setID(int tmp);
    void setBF(int tmp);
    void setLSC(int tmp);
    void setBCNO(QString tmp);
    void setBRNO(QString tmp);
    void setCD(QString tmp);
    void setMSG(QString tmp);
    void GetOneLineFromString(QString strModel);
};

#endif // BUSLINE_H
