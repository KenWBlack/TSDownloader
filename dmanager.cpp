#include "dmanager.h"

DManager::DManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<DownLoader::DOWNINF>("DownLoader::DOWNINF");
    for(int i=0;i<3;i++)
    {
        pthread[i] = new QThread(this);
        dm[i] = new DownLoader(i);
        dm[i]->moveToThread(pthread[i]);
        connect(this,&DManager::dispathWork,dm[i],&DownLoader::doWork);
        connect(dm[i],&DownLoader::sendResult,this,&DManager::getResult);
    }
    for(int i=0;i<3;i++)pthread[i]->start();
}

void DManager::getData(QString url, int start, int end, int  timeout)
{
    this->url = url;
    this->start = start;
    this->end = end;
    this->timeout = timeout;

    DownLoader::DOWNINF temp;
    if(start<0 || end  <0 || timeout<0)
    {
        qDebug()<<"Error start or end num < 0";
        return;
    }
    int index=0,i;
    QString s=url;
    qDebug()<<url.indexOf("*")<<url;
    for(int i=0;i<3;i++)
    {
        pthread[i]->start();
    }
    if(s.indexOf('*') != -1)
    {
        for(i=start;i<=end;i++)
        {
            s=url;
            index  = s.lastIndexOf("*");
            s.replace(index,1,QString::number(i));
            temp.name=i;
            temp.url=QUrl(s);
            temp.timeout=timeout;
            downQueue.enqueue(temp);
        }
    }
    else
    {
        //qDebug()<<"No *";
        temp.url=QUrl(s);
        temp.timeout=timeout;
        downQueue.enqueue(temp);
        qDebug()<<downQueue.isEmpty();
    }

    for(int i=0;(!downQueue.isEmpty()) && i<3;i++)
    {
        qDebug()<<"emit downNext:"<<i;
        pthread[i]->start();
        emit dispathWork(downQueue.dequeue(),i);
    }
}

void DManager::getResult(bool,int num)
{

    if(!downQueue.isEmpty())
    {
        dispathWork(downQueue.dequeue(),num);
    }
    emit sendSucc();
    qDebug()<<"DMThread:"<<QThread::currentThreadId()<<"emit sendSucc";
}
