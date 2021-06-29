#include "configw.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("energyXT2");
    a.setOrganizationName("XT Software AS");
    Config w;
    w.show();
    return a.exec();
}
