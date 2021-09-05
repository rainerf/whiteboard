// This file is part of Whiteboard.
//
// Whiteboard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Whiteboard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Whiteboard.  If not, see <https://www.gnu.org/licenses/>.

#include "wb_mainwindow.h"
#include "wb_paint_window.h"

#include <QSettings>

namespace detail {
constexpr char GEOMETRY_SETTING[] = "paintWindow/geometry";
}

WB_MainWindow::WB_MainWindow(TabletApplication &app, QWidget *parent) : QMainWindow(parent), m_paintWindow(new WB_PaintWindow(app)) {
    setCentralWidget(m_paintWindow);
    setWindowTitle("[*] Whiteboard");
    setWindowIcon(QIcon(":/icons/icons/wysiwyg_black_24dp.svg"));
    QSettings settings;
    restoreGeometry(settings.value(detail::GEOMETRY_SETTING).toByteArray());

    connect(m_paintWindow, &WB_PaintWindow::fileModified, [this](bool x) { setWindowModified(x); });
}

void WB_MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings;
    settings.setValue(detail::GEOMETRY_SETTING, saveGeometry());
    m_paintWindow->closeEvent(event);
}
