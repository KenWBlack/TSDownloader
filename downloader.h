#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QDebug>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QQueue>
#include <QByteArray>
#include <QFile>

class DownLoader : public QObject
{
    Q_OBJECT
public:
    explicit DownLoader(QObject *parent = nullptr);
    DownLoader(int num);

    typedef  struct downInf
        {
            QUrl  url;
            int name;//this number named download file
            int timeout;
        }DOWNINF;

private:
    QUrl url;
    int timeout;
    QNetworkRequest *prequest;
    QNetworkReply *pReply;
    QNetworkAccessManager *pmanager;
    QEventLoop *ploop;
    QTimer *ptimer;
    QByteArray bytes;
    uint8_t retry = 0;
    int num;
    void Retry();
    bool savetoFile(QString,QByteArray);
    bool savetoFile(QUrl);
signals:
    void sendResult(bool,int);
public   slots:
    void doWork(DOWNINF,int);
};

#endif // DOWNLOADER_H
