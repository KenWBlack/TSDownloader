#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "dmanager.h"
#include "downloader.h"
#include <QThread>
#include <QBitmap>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QThread *pthread;
    DManager *pmanager;
    Ui::MainWindow *ui;
    float total,count;
signals:
    void sendWork(QString,int,int,int);
private slots:
    void on_downBtn_clicked();
    void setProgress();
};
#endif // MAINWINDOW_H
