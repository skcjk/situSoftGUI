#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogInit();
    //设置窗口标题和大小
    this->setWindowTitle("串口通信Demo");
    this->resize(800,600);

    serialInit();
    searchCOM();


    //绑定信号和槽
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(ui->send_data,SIGNAL(clicked(bool)),this,SLOT(sendData()));
    connect(ui->open_serialport,SIGNAL(clicked(bool)),this,SLOT(openSerialport()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::sendData()
{
    //获取需要发送的文本
    QString senData=ui->lineEdit->text().trimmed();
    //使用字节数组处理输入的文本
    QByteArray sendDataArray;
    sendDataArray.append(senData.toUtf8().data());
    //使用串口发送字节数组
    serialPort->write(sendDataArray);
}



void MainWindow::receiveData()
{
    //接收其他串口发送过来的数据
    QByteArray serialData;
    serialData.append(serialPort->readAll());
    //把数据依次显示到文本控件上面去
    ui->textEdit->append(serialData);
}




void MainWindow::openSerialport()
{
    //判断串口是否已经打开,若打开则关闭，同时改变状态灯和按钮文字设置为“打开串口”
    if(serialPort->isOpen()){
        serialPort->clear();
        serialPort->close();
        ui->open_serialport->setText("打开串口");
        ui->label->setStyleSheet("background-color: rgb(255, 0, 0);");
    }else {
        //若串口没有打开，则设置指示灯为绿色，设置按钮文字为“关闭串口”
        ui->label->setStyleSheet("background-color:rgb(0,255,0);border-radius:12px;");
        ui->open_serialport->setText("关闭串口");
        serialPort->setPortName(ui->comboBox->currentText());
        serialPort->open(QIODevice::ReadWrite);
        serialPort->setBaudRate(QSerialPort::Baud115200);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);



    }
}

void MainWindow::serialInit()
{
    //初始化串口对象
    serialPort=new QSerialPort(this);
}

void MainWindow::searchCOM()
{
    //获取所有可用端口列表
    QList<QSerialPortInfo> serialPortList=QSerialPortInfo::availablePorts();


    //如果没有可用串口，则在状态栏提示相应信息
    if(serialPortList.isEmpty()){
        this->statusBar()->showMessage("没有可用串口，请插入串口或重启电脑重试");
        m_dialog->showDialog();
    }else {
        m_dialog->showDialog();
    }
}

void MainWindow::dialogInit()
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QWidget *widget = new QWidget;
    layout->addWidget(widget);

    QWidget *canvas = new QWidget;
    canvas->setStyleSheet("QWidget { background: white; }");
    layout->addWidget(canvas);

    // ui->setupUi(this);
    layout->setContentsMargins(20, 20, 20, 20);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(300);

    m_dialog->setParent(this);

    QWidget *dialogWidget = new QWidget;
    QVBoxLayout *dialogWidgetLayout = new QVBoxLayout;
    dialogWidget->setLayout(dialogWidgetLayout);

    QtMaterialFlatButton *closeButton = new QtMaterialFlatButton("Close");
    dialogWidgetLayout->addWidget(closeButton);
    dialogWidgetLayout->setAlignment(closeButton, Qt::AlignBottom | Qt::AlignCenter);

    closeButton->setMaximumWidth(150);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    m_dialog->setWindowLayout(dialogLayout);

    dialogWidget->setMinimumHeight(300);

    dialogLayout->addWidget(dialogWidget);

    connect(closeButton, SIGNAL(pressed()), m_dialog, SLOT(hideDialog()));
}
