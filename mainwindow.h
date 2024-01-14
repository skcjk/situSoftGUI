#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qtmaterialdialog.h>
#include <qtmaterialflatbutton.h>
#include "./ui_mainwindow.h"
#include <qtmaterialiconbutton.h>
#include <qtmaterialappbar.h>
#include <lib/qtmaterialtheme.h>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendData();//发送串口数据
    void receiveData();//接收串口数据
    void openSerialport();//串口开启/关闭控制

private:
    void serialInit();
    void searchCOM();
    void dialogInit();
    void uiInit();
    void qtMaterialFlatButtonInit(QtMaterialFlatButton *thisButton);
    void appBarInit();
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QtMaterialDialog *m_dialog = new QtMaterialDialog;
    QLabel *appBarLabel = new QLabel("Inbox");
    QLabel *appBarTime = new QLabel(QTime::currentTime().toString());
    QLabel *dialogLabel = new QLabel("Your centered text here");
    QtMaterialIconButton *refreshCOMButton = new QtMaterialIconButton(QIcon("refresh.svg"));


    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
};
#endif // MAINWINDOW_H
