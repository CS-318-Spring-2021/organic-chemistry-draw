
#include "MainWindow.h"

#include <QApplication>

int Main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
