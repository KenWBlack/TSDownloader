#ifndef DMANAGER_H
#define DMANAGER_H

#include <QObject>
#include <QThread>
#include <QUrl>
#include <QMetaType>
#include "downloader.h"

class DManager : public QObject
{
    Q_OBJECT
public:
    explicit DManager(QObject *parent = nullptr);
private:
    DownLoader *dm[3];
    QThread *pthread[3];
    int timeout,start,end;
    QString url;
    QQueue<DownLoader::DOWNINF> downQueue;
signals:
    void dispathWork(DownLoader::DOWNINF,int);
    void sendSucc();
public slots:
    void getData(QString,int,int,int);//Get Data from MainThread,url,startnum,endnum,timeout
    void getResult(bool,int);
};

#endif // DMANAGER_H
