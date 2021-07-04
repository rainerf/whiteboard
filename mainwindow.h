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

    void setupToolSelectors();
    void setupUiActions();
    void setupFontToolbar();
    void setupColorActions();
    void setupPenActions();

    ColorAction *addColorAction(QColor const &color, QActionGroup *selector);
    PenAction *addPenAction(int thickness, QActionGroup *selector);

    void saveToFile();
    void loadFromFile();
};
#endif // MAINWINDOW_H
