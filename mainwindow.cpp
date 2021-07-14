#include "mainwindow.h"

#include <QColor>
#include <QFileDialog>
#include <QFontComboBox>
#include <QLabel>
//#include <QGLWidget>

#include "actions/coloraction.h"
#include "actions/penaction.h"
#include "ui_mainwindow.h"

namespace detail {

QPixmap createTexture() {
    constexpr int GRID_SIZE = 40;
    QPixmap pixmap(QSize(GRID_SIZE, GRID_SIZE));
    pixmap.fill();

    QPainter painter(&pixmap);
    QPen penHLines(QColor(200, 200, 200), 1, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
    painter.setPen(penHLines);
    painter.drawLine(0, 0, 0, GRID_SIZE);
    painter.drawLine(0, 0, GRID_SIZE, 0);
    painter.end();

    return pixmap;
}

} // namespace detail

void MainWindow::setupToolSelectors() {
    auto *toolSelector = new QActionGroup(this);
    toolSelector->setExclusive(true);
    toolSelector->addAction(ui->actionText);
    toolSelector->addAction(ui->actionHighlight);
    toolSelector->addAction(ui->actionPointer);
    toolSelector->addAction(ui->actionPen);
    toolSelector->addAction(ui->actionZoomTool);
    toolSelector->addAction(ui->actionPanTool);

    connect(ui->actionPen, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setPenTool);
    connect(ui->actionText, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setTextTool);
    connect(ui->actionHighlight, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setHighlightTool);
    connect(ui->actionPointer, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setPointerTool);
    connect(ui->actionZoomTool, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setZoomTool);
    connect(ui->actionPanTool, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setPanTool);

    connect(ui->actionDelete, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::deleteSelectedItems);

    // disable changing the tool while one is in use
    connect(ui->graphicsView, &WhiteBoardGraphicsView::toolInUse, toolSelector, &QActionGroup::setDisabled);
}

void MainWindow::setupUiActions() {
    connect(ui->actionCopy, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::selectAll);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(ui->actionClear, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::clear);
    connect(ui->actionZoomOriginal, &QAction::triggered, [=]() { ui->graphicsView->setScale(1); });
    connect(ui->actionZoomFit, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::zoomToFit);
    // connect(ui->actionHighlight, &QAction::toggled, [=](bool x) { m_colorSelector->setEnabled(!x); });
}

void MainWindow::setupFontToolbar() {
    auto *fontSelectionBox = new QFontComboBox();
    ui->toolBarFont->addWidget(fontSelectionBox);
    connect(fontSelectionBox, &QFontComboBox::currentFontChanged, ui->graphicsView, &WhiteBoardGraphicsView::setFont);

    auto *fontSizeBox = new QComboBox();
    ui->toolBarFont->addWidget(fontSizeBox);
    fontSizeBox->insertItems(0, {"4", "6", "8", "10", "12", "16", "20", "24", "36", "48"});
    fontSizeBox->setEditable(true);
    connect(fontSizeBox, &QComboBox::currentTextChanged, [=](QString const &text) {
        if (int size = text.toInt())
            ui->graphicsView->setFontSize(size);
    });
    fontSizeBox->setCurrentText("20");
}

MainWindow::MainWindow(TabletApplication &app, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    auto *scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(detail::createTexture()));

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    setupToolSelectors();
    setupColorActions();
    setupPenActions();
    setupUiActions();
    setupFontToolbar();

    // ui->graphicsView->setViewport(new QGLWidget);
    // ui->graphicsView->setRenderHint(QPainter::HighQualityAntialiasing);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    // default settings
    ui->actionPen->trigger();

    qApp->installEventFilter(this);
    connect(&app, &TabletApplication::sendTabletActive, this, &MainWindow::tabletActive);
}

QLabel *createToolbarHeader(QString const &name) {
    auto *item = new QLabel(name);
    item->setAlignment(Qt::AlignCenter);
    item->setMargin(2);
    return item;
}

void MainWindow::setupColorActions() {
    ui->toolBarSettings->addWidget(createToolbarHeader("Color"));

    auto *colorSelector = new QActionGroup(this);
    colorSelector->setExclusive(true);

    addColorAction(QColor(0, 0, 0), colorSelector)->trigger();
    addColorAction(QColor(128, 128, 128), colorSelector);
    addColorAction(QColor(240, 0, 0), colorSelector);
    addColorAction(QColor(0, 240, 0), colorSelector);
    addColorAction(QColor(0, 0, 240), colorSelector);
    addColorAction(QColor(240, 240, 0), colorSelector);
    addColorAction(QColor(240, 0, 240), colorSelector);
    addColorAction(QColor(0, 240, 240), colorSelector);
    addColorAction(QColor(240, 128, 0), colorSelector);
    addColorAction(QColor(240, 0, 128), colorSelector);
    addColorAction(QColor(0, 240, 128), colorSelector);
    addColorAction(QColor(128, 240, 0), colorSelector);
    addColorAction(QColor(128, 0, 240), colorSelector);
    addColorAction(QColor(0, 128, 240), colorSelector);
}

void MainWindow::setupPenActions() {
    ui->toolBarSettings->addWidget(createToolbarHeader("Pen"));

    auto *penSelector = new QActionGroup(this);
    penSelector->setExclusive(true);

    addPenAction(10, penSelector);
    addPenAction(5, penSelector)->trigger();
    addPenAction(1, penSelector);
}

ColorAction *MainWindow::addColorAction(const QColor &color, QActionGroup *selector) {
    auto *newAction = new ColorAction(color, this);
    connect(newAction, &ColorAction::colorSelected, ui->graphicsView, &WhiteBoardGraphicsView::setColor);
    ui->toolBarSettings->addAction(newAction);
    selector->addAction(newAction);
    return newAction;
}

PenAction *MainWindow::addPenAction(int thickness, QActionGroup *selector) {
    auto *newAction = new PenAction(thickness, this);
    connect(newAction, &PenAction::penSelected, ui->graphicsView, &WhiteBoardGraphicsView::setPenThickness);
    ui->toolBarSettings->addAction(newAction);
    selector->addAction(newAction);
    return newAction;
}

void MainWindow::saveToFile() {
    QFileDialog dialog(this, "Save Whiteboard", "", "Whiteboard (*.whb);;All Files (*)");
    dialog.setDefaultSuffix(".whb");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() == QDialog::Accepted) {
        const auto filename = dialog.selectedFiles().front();
        ui->graphicsView->saveToFile(filename);
    }
}

void MainWindow::loadFromFile() {
    QFileDialog dialog(this, "Open Whiteboard", "", "Whiteboard (*.whb);;All Files (*)");
    dialog.setDefaultSuffix(".whb");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec() == QDialog::Accepted) {
        const auto filename = dialog.selectedFiles().front();
        ui->graphicsView->loadFromFile(filename);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease) {
        if (obj == ui->graphicsView->viewport()) {
            return m_tabletActive;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow() {
    delete ui;
}
