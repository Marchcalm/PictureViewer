#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "pub_subwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    for (int i = 0; i < argc; ++i) {
        qDebug() << i << argv[i];
    }

    MainWindow w;
    w.show();

    return a.exec();
}
