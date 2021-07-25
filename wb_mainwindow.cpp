#include "wb_mainwindow.h"
#include "wb_paint_window.h"

WB_MainWindow::WB_MainWindow(TabletApplication &app, QWidget *parent) : QMainWindow(parent) {
    setCentralWidget(new WB_PaintWindow(app));
    setWindowTitle("Whiteboard");
    setWindowIcon(QIcon(":/icons/icons/wysiwyg_black_24dp.svg"));
}
