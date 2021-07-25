#include "wb_mainwindow.h"
#include "wb_paint_window.h"

#include <QSettings>

namespace detail {
constexpr char GEOMETRY_SETTING[] = "paintWindow/geometry";
}

WB_MainWindow::WB_MainWindow(TabletApplication &app, QWidget *parent) : QMainWindow(parent), m_paintWindow(new WB_PaintWindow(app)) {
    setCentralWidget(m_paintWindow);
    setWindowTitle("Whiteboard");
    setWindowIcon(QIcon(":/icons/icons/wysiwyg_black_24dp.svg"));
    QSettings settings;
    restoreGeometry(settings.value(detail::GEOMETRY_SETTING).toByteArray());
}

void WB_MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings;
    settings.setValue(detail::GEOMETRY_SETTING, saveGeometry());
    m_paintWindow->closeEvent(event);
}
