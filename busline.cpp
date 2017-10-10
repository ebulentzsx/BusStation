#include "busline.h"

BusLine::BusLine()
{
    id=-1;
}

void BusLine::setID(int tmp)
{
    id0=id;
    id=tmp;
}

void BusLine::setBF(int tmp)
{
    BusFoward0=BusFoward;
    BusFoward=tmp;
}

void BusLine::setLSC(int tmp)
{
    LSC0=LSC;
    LSC=tmp;
}

void BusLine::setBCNO(QString tmp)
{
    BCNO0=BCNO;
    BCNO=tmp;
}

void BusLine::setBRNO(QString tmp)
{
    BRNO0=BRNO;
    BRNO=tmp;
}

void BusLine::setCD(QString tmp)
{
    CD0=CD;
    CD = tmp;
}

void BusLine::setMSG(QString tmp)
{
    MSG0=MSG;
    MSG=tmp;
}

void BusLine::GetOneLineFromString(QString strModel)
{

}
