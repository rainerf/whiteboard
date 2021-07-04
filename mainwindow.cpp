#include <QColor>
#include <QFontComboBox>
#include <QLabel>
//#include <QGLWidget>

#include "mainwindow.h"

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
    m_toolSelector = new QActionGroup(this);
    m_toolSelector->setExclusive(true);
    m_toolSelector->addAction(ui->actionText);
    m_toolSelector->addAction(ui->actionHighlight);
    m_toolSelector->addAction(ui->actionPointer);
    m_toolSelector->addAction(ui->actionPen);

    connect(ui->actionPen, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setPenTool);
    connect(ui->actionText, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setTextTool);
    connect(ui->actionHighlight, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setHighlightTool);
    connect(ui->actionPointer, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::setPointerTool);
}

void MainWindow::setupUiActions() {
    connect(ui->actionDelete, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::deleteSelectedItems);
    connect(ui->actionCopy, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::selectAll);
    connect(ui->actionSave, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::saveToFile);
    connect(ui->actionOpen, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::loadFromFile);
    connect(ui->actionClear, &QAction::triggered, ui->graphicsView, &WhiteBoardGraphicsView::clear);
    connect(ui->actionZoomOriginal, &QAction::triggered, [=]() { ui->graphicsView->setScale(1); });
    // connect(ui->actionHighlight, &QAction::toggled, [=](bool x) { m_colorSelector->setEnabled(!x); });
}

void MainWindow::setupFontToolbar() {
    auto fontSelectionBox = new QFontComboBox();
    ui->toolBarFont->addWidget(fontSelectionBox);
    connect(fontSelectionBox, &QFontComboBox::currentFontChanged, ui->graphicsView, &WhiteBoardGraphicsView::setFont);

    auto fontSizeBox = new QComboBox();
    ui->toolBarFont->addWidget(fontSizeBox);
    fontSizeBox->insertItems(0, {"4", "6", "8", "10", "12", "16", "20", "24", "36", "48"});
    fontSizeBox->setEditable(true);
    connect(fontSizeBox, &QComboBox::currentTextChanged, [=](QString const &text) {
        if (int size = text.toInt())
            ui->graphicsView->setFontSize(size);
    });
    fontSizeBox->setCurrentText("20");
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(QBrush(detail::createTexture()));

    ui->setupUi(this);
    ui->graphicsView->setScene(m_scene);

    setupToolSelectors();
    setupColorActions();
    setupPenActions();
    setupUiActions();
    setupFontToolbar();

    ui->toolBarTools->addAction(ui->actionDelete);

    // ui->graphicsView->setViewport(new QGLWidget);
    // ui->graphicsView->setRenderHint(QPainter::HighQualityAntialiasing);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    // default settings
    ui->actionPen->trigger();
}

void MainWindow::setupColorActions() {
    ui->toolBarTools->addWidget(new QLabel("Color"));

    m_colorSelector = new QActionGroup(this);
    m_colorSelector->setExclusive(true);

    addColorAction(QColor(0, 0, 0))->trigger();
    addColorAction(QColor(128, 128, 128));
    addColorAction(QColor(240, 0, 0));
    addColorAction(QColor(0, 240, 0));
    addColorAction(QColor(0, 0, 240));
    addColorAction(QColor(240, 240, 0));
    addColorAction(QColor(240, 0, 240));
    addColorAction(QColor(0, 240, 240));
    addColorAction(QColor(240, 128, 0));
    addColorAction(QColor(240, 0, 128));
    addColorAction(QColor(0, 240, 128));
    addColorAction(QColor(128, 240, 0));
    addColorAction(QColor(128, 0, 240));
    addColorAction(QColor(0, 128, 240));
}

void MainWindow::setupPenActions() {
    ui->toolBarTools->addWidget(new QLabel("Tool"));

    m_penSelector = new QActionGroup(this);
    m_penSelector->setExclusive(true);

    addPenAction(10);
    addPenAction(5)->trigger();
    addPenAction(1);
}

ColorAction *MainWindow::addColorAction(const QColor &color) {
    ColorAction *newAction = new ColorAction(color, this);
    connect(newAction, &ColorAction::colorSelected, ui->graphicsView, &WhiteBoardGraphicsView::setColor);
    ui->toolBarTools->addAction(newAction);
    m_colorSelector->addAction(newAction);
    return newAction;
}

PenAction *MainWindow::addPenAction(int thickness) {
    PenAction *newAction = new PenAction(thickness, this);
    connect(newAction, &PenAction::penSelected, ui->graphicsView, &WhiteBoardGraphicsView::setPenThickness);
    ui->toolBarTools->addAction(newAction);
    m_penSelector->addAction(newAction);
    return newAction;
}

MainWindow::~MainWindow() {
    delete ui;
}
