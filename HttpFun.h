#ifndef HTTPFUN_H
#define HTTPFUN_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
#include <QSslConfiguration>
#include <QString>
#include <QByteArray>
#include "pubulicDefine.h"
#define MY_GB2312 0
#define MY_UTF8 0
#define NOINCODE 1
//http请求类
class HttpFun : public QObject
{
    Q_OBJECT
public:
    explicit HttpFun(QObject *parent = 0);
    void sendRequest(const QString& strUrl);//根据url发起http请求
signals:
    void signal_requestFinished(bool bSuccess,const QString& strResult); //http请求结束
public slots:
    void slot_requestFinished(); //http请求结束
    void slot_requestTimeout();  //请求超时
    void slot_sendRequest(const QString& strUrl);


private:
    QNetworkAccessManager* m_pNetworkManager;//网络管理类
    QNetworkReply* m_pNetworkReply; //封装请求返回信息
    QTimer* m_pTimer; //请求超时计时器
    QString m_strUrl; //记录当前请求的url
};

#endif // HTTPFUN_H
