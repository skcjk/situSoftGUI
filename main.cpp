#include <QApplication>
#include "framelessWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    framelessWidget w;
    w.setWindowFlag(Qt::FramelessWindowHint); //无边框
    w.setAttribute(Qt::WA_TranslucentBackground);  //透明
    w.show();
    return a.exec();
}
