#include "HttpFun.h"
#include <QTextCodec>
#include <qdebug.h>
const int nHTTP_TIME = 10000; //10秒

HttpFun::HttpFun(QObject *parent) :
    QObject(parent)
{
    m_pNetworkManager = new QNetworkAccessManager(this);
    m_pNetworkReply = NULL;
    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));//超时信号
}

//发起请求
void HttpFun::sendRequest(const QString &strUrl)
{
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    {
        QSslConfiguration sslConfig;
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
       // sslConfig.setProtocol(QSsl::TlsV1_1);
        //QSsl::TlsV1
        sslConfig.setProtocol(QSsl::TlsV1);
        netRequest.setSslConfiguration(sslConfig);
    }
//    QString strBody; //http body部分，可封装参数信息
//    QByteArray contentByteArray = strBody.toLatin1();//转成二进制
//    m_pNetworkReply = m_pNetworkManager->post(netRequest,contentByteArray);//发起post请求

    m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
    connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号
    m_pTimer->start(nHTTP_TIME);
    qDebug()<<"request is going！";

}

//请求结束
void HttpFun::slot_requestFinished()
{
    m_pTimer->stop();//关闭定时器
    QByteArray resultContent = m_pNetworkReply->readAll();
    //Chinese code could be change here!!-------
    //--------------------------------------------------------------------------------

    //     QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());//change the char code

    //--------------------------------------------------------------------------------
#ifdef MY_GB2312
    QTextCodec* pCodec = QTextCodec::codecForName("GB2312");
    QString strResult = pCodec->toUnicode(resultContent);
#endif

#ifdef MY_UTF8
    QTextCodec* pCodec = QTextCodec::codecForName("UTF-8");
    QString strResult = pCodec->toUnicode(resultContent);
#endif
#ifdef NOINCODE
    QString strResult = resultContent;
#endif

    //strResult=resultContent.data();
    //QString strResult=QString:: resultContent
    int nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if(nHttpCode == 200)//成功
    {
       //qDebug()<<"To Hex:"<<resultContent.toHex();
       //qDebug()<<"String:"<<strResult;
        //emit signal_requestFinished(true,strResult);//请求成功
        emit signal_requestFinished(true,resultContent);
         //纯文本显示


    }
    else
    {
        emit signal_requestFinished(false,strResult);//请求失败
        qDebug()<<"请求失败！----------request fail";
    }
    m_pNetworkReply->deleteLater();
    this->deleteLater(); //释放内存
}

//请求超时
void HttpFun::slot_requestTimeout()
{
    emit signal_requestFinished(false,"timeout");//请求失败
    m_pNetworkReply->deleteLater();
    this->deleteLater();//释放内存
}

void HttpFun::slot_sendRequest(const QString &strUrl)
{
    sendRequest(strUrl);
}
