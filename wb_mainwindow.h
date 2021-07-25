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
