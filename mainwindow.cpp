#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    total=count=0;
    ui->setupUi(this);
    ui->FileBtn->setIcon(QIcon(":/icon/Folder Attempt Generic.ico"));
    QPixmap *icon = new QPixmap(":/icon/Folder Attempt Generic.ico");
    ui->FileBtn->setMask(icon->mask());
    ui->StartText->setText(tr("0"));
    ui->EndText->setText(tr("0"));
    ui->TimeText->setText(tr("1"));
    ui->TimeText->setToolTip(tr("下载超时时间，单位：秒"));
    pthread  = new QThread;
    //pmanager = new DownLoader(0);
    pmanager = new DManager;
    pmanager->moveToThread(pthread);
    connect(pthread, &QThread::finished, pmanager, &QObject::deleteLater);
    connect(this,&MainWindow::sendWork,pmanager,&DManager::getData);
    connect(pmanager,&DManager::sendSucc,this,&MainWindow::setProgress);
    pthread->start();
    qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_downBtn_clicked()
{
    int start = ui->StartText->text().toInt(),end = ui->EndText->text().toInt();
    total = start-end+1;
    count=0;
    QString urltext = ui->UrlText->text(),s;
    if(urltext.isEmpty())return;
    if(urltext.startsWith("http://") || urltext.startsWith("https://"))
    {
        s=urltext;
    }
    else s = QString("http://")+urltext;
    emit sendWork(s,start,end,ui->TimeText->text().toInt()*1000);
    qDebug()<<"MainThread:"<<QThread::currentThreadId()<<"emit send";
    ui->statusbar->showMessage("正在下载",1000);
}

void MainWindow::setProgress()
{
    count++;
    ui->progressBar->setValue((int)(count/total)*100);
}
