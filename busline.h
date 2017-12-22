#ifndef BUSLINE_H
#define BUSLINE_H
#include <QString>
#include "pubulicDefine.h"
class BusLine
{

public:
    BusLine();
    int flag;
    int id;
    int numPort;
    int BusFoward,BusFoward0;  // back or forward
    int LSC;  //left stations  2
    QString BCNO; //BG4532
    QString BRNO;//line name  325
    QString CD;//distance  1800m
    QString MSG;
    QString BCSta;
    QString STime;
    QString ESN;
     QString VNO;

    void setID(int tmp);
    void setBF(int tmp);
    void setLSC(int tmp);
    void setBCNO(QString tmp);
    void setBRNO(QString tmp);
    void setCD(QString tmp);
    void setMSG(QString tmp);

};

#endif // BUSLINE_H
