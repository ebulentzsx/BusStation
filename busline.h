#ifndef BUSLINE_H
#define BUSLINE_H
#include <QString>

class BusLine
{

public:
    BusLine();
    int flag;
    int id,id0;
    int BusFoward,BusFoward0;  // back or forward
    int LSC,LSC0;  //left stations  2
    QString BCNO,BCNO0; //BG4532
    QString BRNO,BRNO0;//line name  325
    QString CD,CD0;//distance  1800m
    QString MSG,MSG0;
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
