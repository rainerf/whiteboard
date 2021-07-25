#ifndef WB_MAINWINDOW_H
#define WB_MAINWINDOW_H

#include <QMainWindow>
#include "lib/tablet_application.h"

class WB_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit WB_MainWindow(TabletApplication &app, QWidget *parent = nullptr);


};

#endif // WB_MAINWINDOW_H
