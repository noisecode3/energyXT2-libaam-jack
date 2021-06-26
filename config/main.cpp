#include <QtGui/QApplication>
#include "configw.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("energyXT2");
    app.setOrganizationName("XT Software AS");
    Config w;
    w.show();

    return app.exec();
}
