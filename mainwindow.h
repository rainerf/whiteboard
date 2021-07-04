#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QGraphicsScene>
#include <QMainWindow>

class ColorAction;
class PenAction;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    QActionGroup *m_colorSelector;
    QActionGroup *m_toolSelector;
    QActionGroup *m_penSelector;

    void setupToolSelectors();
    void setupUiActions();
    void setupFontToolbar();
    void setupColorActions();
    void setupPenActions();

    ColorAction *addColorAction(QColor const &color);
    PenAction *addPenAction(int thickness);
};
#endif // MAINWINDOW_H
