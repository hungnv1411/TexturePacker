#include "MainWindow.hpp"
#include <QApplication>

#define APP_VERSION "1.0"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("fotoapps");
    QCoreApplication::setApplicationName("TexturePacker");
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
