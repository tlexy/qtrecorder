#include "widget.h"
#include <QApplication>
#include <thread>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");

    QApplication a(argc, argv);
    Widget w;

    w.setWindowTitle(QString("录音机"));
    w.resize(410, 230);
    w.show();

    return a.exec();
}
