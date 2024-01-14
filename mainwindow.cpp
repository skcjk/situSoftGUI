#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    uiInit();
    serialInit();
    searchCOM();


    //绑定信号和槽
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(ui->send_data,SIGNAL(clicked(bool)),this,SLOT(sendData()));
    connect(ui->open_serialport,SIGNAL(clicked(bool)),this,SLOT(openSerialport()));
    connect(ui->searchButton,&QtMaterialFlatButton::clicked,m_dialog,&QtMaterialDialog::showDialog);

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

    static bool flag = true; //第一次运行
    //如果没有可用串口，则在状态栏提示相应信息
    if(serialPortList.isEmpty()){
        dialogLabel->setText("没有可用串口，请插入串口或重启电脑重试");
        appBarLabel->setText("没有可用串口");
        m_dialog->showDialog();
    }else {
        if (flag)
            dialogLabel->setText("可用串口数量为:"+QString::number( serialPortList.count()) );
        appBarLabel->setText("可用串口数量为:"+QString::number( serialPortList.count()) );
        m_dialog->showDialog();
        //创建遍历迭代器，把串口遍历显示到下拉框中
        QList<QSerialPortInfo>::Iterator nextSerialPort=serialPortList.begin();
        ui->comboBox->clear();
        while (nextSerialPort != serialPortList.end()) {
            ui->comboBox->addItem(nextSerialPort->portName());
            nextSerialPort++;

        }
    }
}

void MainWindow::dialogInit()
{
    QVBoxLayout *layout = new QVBoxLayout;
    // setLayout(layout);

    QWidget *widget = new QWidget;
    layout->addWidget(widget);

    QWidget *canvas = new QWidget;
    canvas->setStyleSheet("QWidget { background: white; }");
    layout->addWidget(canvas);

    // ui->setupUi(this);
    layout->setContentsMargins(20, 20, 20, 20);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(150);

    m_dialog->setParent(this);

    QWidget *dialogWidget = new QWidget; //显示内容
    QVBoxLayout *dialogWidgetLayout = new QVBoxLayout;
    dialogWidget->setLayout(dialogWidgetLayout);

    QFont textFont("Microsoft YaHei", 12, QFont::Bold);
    dialogLabel->setFont(textFont);
    dialogLabel->setAlignment(Qt::AlignCenter);
    dialogWidgetLayout->addWidget(dialogLabel, 0, Qt::AlignCenter);

    QtMaterialFlatButton *closeButton = new QtMaterialFlatButton("Close"); //关闭按钮
    qtMaterialFlatButtonInit(closeButton);
    dialogWidgetLayout->addWidget(closeButton);
    dialogWidgetLayout->setAlignment(closeButton, Qt::AlignBottom | Qt::AlignCenter);
    closeButton->setMaximumWidth(300);
    // QPalette pal = widget->palette();
    // pal.setColor(QPalette::Button, QColor(240, 240, 240));
    // closeButton->setAutoFillBackground(true);  // 允许部件自动填充背景
    // closeButton->setPalette(pal);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    m_dialog->setWindowLayout(dialogLayout);

    dialogWidget->setMinimumHeight(150);

    dialogLayout->addWidget(dialogWidget);

    connect(closeButton, SIGNAL(pressed()), m_dialog, SLOT(hideDialog()));
    connect(refreshCOMButton,&QtMaterialIconButton::clicked, this,&MainWindow::searchCOM);
}

void MainWindow::uiInit()
{
    dialogInit();
    ui->setupUi(this);
    qtMaterialFlatButtonInit(ui->searchButton);
    appBarInit();
    //设置窗口标题和大小
    this->setWindowTitle("串口通信Demo");
    this->resize(800,600);
}

void MainWindow::qtMaterialFlatButtonInit(QtMaterialFlatButton *thisButton)
{
    QFont font("Microsoft YaHei", 12, QFont::Bold);
    thisButton->setRole(Material::Primary);
    thisButton->setRippleStyle(Material::CenteredRipple);
    thisButton->setFont(font);
}

void MainWindow::appBarInit()
{
    QtMaterialAppBar *m_appBar = new QtMaterialAppBar;
    appBarLabel->setAttribute(Qt::WA_TranslucentBackground);
    appBarLabel->setForegroundRole(QPalette::WindowText);
    appBarLabel->setContentsMargins(6, 0, 0, 0);
    appBarTime->setAttribute(Qt::WA_TranslucentBackground);
    appBarTime->setForegroundRole(QPalette::WindowText);
    appBarTime->setAlignment(Qt::AlignCenter);

    QPalette palette = appBarLabel->palette();
    palette.setColor(appBarLabel->foregroundRole(), Qt::white);
    appBarLabel->setPalette(palette);
    appBarTime->setPalette(palette);

    appBarLabel->setFont(QFont("Microsoft YaHei", 16, QFont::Normal));
    appBarTime->setFont(QFont("Microsoft YaHei", 16, QFont::Normal));
    refreshCOMButton->setIconSize(QSize(24, 24));
    m_appBar->appBarLayout()->addWidget(refreshCOMButton);
    m_appBar->appBarLayout()->addWidget(appBarLabel);
    m_appBar->appBarLayout()->addStretch(1);
    refreshCOMButton->setColor(Qt::white);
    refreshCOMButton->setFixedWidth(42);

    QVBoxLayout *layout = new QVBoxLayout;

    QWidget *widget = new QWidget;
    layout->addWidget(widget);

    QWidget *canvas = new QWidget;
    canvas->setStyleSheet("QWidget { background: white; }");
    layout->addWidget(canvas);

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(300);
    layout->addWidget(m_appBar);
    layout->addStretch(1);
    m_appBar->appBarLayout()->addWidget(appBarTime);
    ui->centralwidget->setLayout(layout);
}
