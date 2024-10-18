#include "mainwindow.h"
#include <QApplication>

#define LENGTH 1 << 10
#define HALFLEN 1 << 9
int y_data[LENGTH];
double f_data[HALFLEN];

int main(int argc, char *argv[])
{
    // 屏幕比例显示
    if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling,true);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
