#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSerialPort>
#include<QSerialPortInfo>

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
    Ui::MainWindow *ui;
    QSerialPort *serialPort;


};
#endif // MAINWINDOW_H
