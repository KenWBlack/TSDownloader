#include "downloader.h"

DownLoader::DownLoader(QObject *parent) : QObject(parent)
{
}

DownLoader::DownLoader(int num)
{
    this->num = num;
    this->timeout=3000;
    ptimer = new QTimer(this);
    ptimer->setInterval(3000); //设置超时时间ms
    //timer.setSingleShot(true); //单次触发
    // 请求网址
     pmanager = new QNetworkAccessManager(this);
     prequest  = new QNetworkRequest;
     ploop =  new  QEventLoop(this);
     retry=4;
     connect(ptimer, &QTimer::timeout, ploop, &QEventLoop::quit);
}

void DownLoader::Retry()
{
    retry++;
    qDebug()<<"Retry:"<<retry;
    disconnect(pReply, &QNetworkReply::finished, ploop, &QEventLoop::quit);
    disconnect(ptimer, &QTimer::timeout, ploop, &QEventLoop::quit);
    pReply->abort();
    pReply->deleteLater();

    ploop->exit();
    ptimer->stop();
    ptimer->setInterval(this->timeout);

    pReply = pmanager->get(*prequest);
    connect(ptimer, &QTimer::timeout, ploop, &QEventLoop::quit);
    connect(pReply, &QNetworkReply::finished, ploop, &QEventLoop::quit);
    ptimer->start();
    ploop->exec();
}

bool DownLoader::savetoFile(QString name,QByteArray data)
{
    /*int index = name.lastIndexOf('/');
    if(index != -1)
    name = name.right(name.length()-index-1);
    */
    QFile fp(name);
    if(fp.exists())
    {

    }
    fp.open(QIODevice::ReadWrite);
    fp.write(data);
    fp.close();
}

bool DownLoader::savetoFile(QUrl)
{
    //Unimplement method
}

void DownLoader::doWork(DownLoader::DOWNINF work, int num)
{
    if(this->num==num)
    {
        qDebug()<<"Thread"<<num<<"doWork";
        this->timeout = work.timeout;
        this->url = work.url;
        ptimer->setInterval(timeout);
        retry=0;
        while(true)
        {
            while (retry<3)
            {
                //qDebug()<<"While";
                prequest->setUrl(this->url);
                prequest->setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64)");
                pReply = pmanager->get(*prequest);
                connect(pReply, &QNetworkReply::finished, ploop, &QEventLoop::quit);

                ptimer->start();
                ploop->exec(); // 启动事件循环

                QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int nStatusCode = variant.toInt();
                // 根据状态码做进一步数据处理
                qDebug()<<"Thread:"<<num<<"StatusCode:"<<nStatusCode;
                if(nStatusCode==200)
                {
                    bytes = pReply->readAll();
                    savetoFile(QString::number(work.name),bytes);
                   // qDebug()<<bytes;
                }

                if (!ptimer->isActive())
                {
                    // 处理超时
                    Retry();
                }
                else
                {
                    // 处理响应
                    ptimer->stop();
                    if (pReply->error() != QNetworkReply::NoError)
                    {
                        // 错误处理
                        qDebug() << "Error String : " << pReply->errorString();
                        Retry();
                    }
                    else
                    {
                        retry = 4;
                        //  emit downSucc(num);
                        qDebug()<<"Thread:"<<num<<" emit downSucc";
                        //this->yieldCurrentThread();
                        //break;
                    }
                }
            }
            if(retry>=3)
            {
                retry++;
                break;
                //emit downFail(num);
            }
        }
    }
}
