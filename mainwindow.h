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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QGraphicsScene>
#include <QMainWindow>
#include "lib/tablet_application.h"

class ColorAction;
class PenAction;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(TabletApplication &app, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void tabletActive(bool x) {
        m_tabletActive = x;
    }

private:
    Ui::MainWindow *ui;
    bool m_tabletActive = false;

    void setupToolSelectors();
    void setupUiActions();
    void setupFontToolbar();
    void setupColorActions();
    void setupPenActions();

    ColorAction *addColorAction(QColor const &color, QActionGroup *selector);
    PenAction *addPenAction(int thickness, QActionGroup *selector);

    void showFileSaveDialog();
    void showFileLoadDialog();
    void loadFromFile(QString filename);
    void addFileToRecentlyUsed(QString filename);
    void updateRecentlyUsedFiles();

    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif // MAINWINDOW_H
