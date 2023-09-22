#include "MVCDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MVCDemo w;
    w.show();
    return a.exec();
}
