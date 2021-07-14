#include "mainwindow.h"
#include "lib/tabletapplication.h"

int main(int argc, char *argv[]) {
    TabletApplication a(argc, argv);
    MainWindow w(a);
    w.show();
    return a.exec();
}
