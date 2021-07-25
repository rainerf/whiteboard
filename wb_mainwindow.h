#ifndef WB_MAINWINDOW_H
#define WB_MAINWINDOW_H

#include <QMainWindow>
#include "lib/tablet_application.h"

class WB_PaintWindow;

class WB_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit WB_MainWindow(TabletApplication &app, QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;

private:
    WB_PaintWindow *m_paintWindow;
};

#endif // WB_MAINWINDOW_H
