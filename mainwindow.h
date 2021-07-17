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

    void saveToFile();
    void loadFromFile();

    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif // MAINWINDOW_H
