#include "busPublic.h"
void outputMessage(QtMsgType type, const char *msg);
int main(int argc, char *argv[])
{
    //qInstallMsgHandler(outputMessage);

    QCoreApplication a(argc, argv);
    testEnv();
    HttpFun *pHttpFun = new HttpFun();
    GetHttpReturn *newInfo = new GetHttpReturn();
    //QString strUrl = "http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey=GBRDBDNO&deviceNumber=1314&busForward=1";
    //QString strUrl="http://113.108.61.26:10000/YiYangIndex.ashx?ActionKey=GCSDT";
    newInfo->SetDevicePosition("1314","1");
    newInfo->GetUrl(1);

    QObject::connect(pHttpFun,SIGNAL(signal_requestFinished(bool,QString)),newInfo,SLOT(slot_requestFinished(bool,QString)));
    pHttpFun->sendRequest(newInfo->strUrl);

    return a.exec();
}


