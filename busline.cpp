#include "busline.h"

BusLine::BusLine()
{
    id=-1;
}

void BusLine::setID(int tmp)
{

    id=tmp;
}

void BusLine::setBF(int tmp)
{
    BusFoward0=BusFoward;
    BusFoward=tmp;
}

void BusLine::setLSC(int tmp)
{

    LSC=tmp;
}

void BusLine::setBCNO(QString tmp)
{

    BCNO=tmp;
}

void BusLine::setBRNO(QString tmp)
{

    BRNO=tmp;
}

void BusLine::setCD(QString tmp)
{

    CD = tmp;
}

void BusLine::setMSG(QString tmp)
{

    MSG=tmp;
}


