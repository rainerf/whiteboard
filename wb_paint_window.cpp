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

#include "wb_paint_window.h"

#include <QColor>
#include <QFileDialog>
#include <QFontComboBox>
#include <QLabel>
#include <QSettings>
#include <QShortcut>
//#include <QGLWidget>

#include "actions/color_action.h"
#include "actions/pen_action.h"
#include "ui_wb_paint_window.h"

void WB_PaintWindow::setupToolSelectors() {
    auto *toolSelector = new QActionGroup(this);
    toolSelector->setExclusive(true);
    toolSelector->addAction(ui->actionText);
    toolSelector->addAction(ui->actionHighlight);
    toolSelector->addAction(ui->actionPointer);
    toolSelector->addAction(ui->actionPen);
    toolSelector->addAction(ui->actionZoomTool);
    toolSelector->addAction(ui->actionPanTool);
    toolSelector->addAction(ui->actionSelectMove);

    connect(ui->actionPen, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setPenTool);
    connect(ui->actionText, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setTextTool);
    connect(ui->actionHighlight, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setHighlightTool);
    connect(ui->actionPointer, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setPointerTool);
    connect(ui->actionZoomTool, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setZoomTool);
    connect(ui->actionPanTool, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setPanTool);
    connect(ui->actionSelectMove, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setSelectTool);

    // disable changing the tool while one is in use
    connect(ui->graphicsView, &WB_GraphicsView::toolInUse, toolSelector, &QActionGroup::setDisabled);
}

void WB_PaintWindow::setupUiActions() {
    connect(ui->actionCopy, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::selectAll);
    connect(ui->actionSave, &QAction::triggered, this, &WB_PaintWindow::showFileSaveDialog);
    connect(ui->actionOpen, &QAction::triggered, this, &WB_PaintWindow::showFileLoadDialog);
    ui->actionOpen->setMenu(new QMenu()); // recently used menu
    updateRecentlyUsedFiles();
    connect(ui->actionClear, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::clear);
    connect(ui->actionZoomOriginal, &QAction::triggered, [=]() { ui->graphicsView->setZoom(1); });
    connect(ui->actionZoomFit, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::zoomToFit);
    // connect(ui->actionHighlight, &QAction::toggled, [=](bool x) { m_colorSelector->setEnabled(!x); });

    auto *undoAction = ui->graphicsView->getUndoStack()->createUndoAction(this);
    undoAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/undo_black_24dp.svg")));
    undoAction->setShortcut(QKeySequence::Undo);
    auto *redoAction = ui->graphicsView->getUndoStack()->createRedoAction(this);
    redoAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/redo_black_24dp.svg")));
    redoAction->setShortcut(QKeySequence::Redo);
    ui->toolBarCommon->addAction(undoAction);
    ui->toolBarCommon->addAction(redoAction);

    auto *deleteShortcut = new QShortcut(QKeySequence("Del"), this);
    connect(deleteShortcut, &QShortcut::activated, ui->graphicsView, &WB_GraphicsView::deleteSelectedItems);
}

void WB_PaintWindow::setupFontToolbar() {
    auto *fontSelectionBox = new QFontComboBox();
    ui->toolBarFont->addWidget(fontSelectionBox);
    connect(fontSelectionBox, &QFontComboBox::currentFontChanged, ui->graphicsView, &WB_GraphicsView::setFont);

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

WB_PaintWindow::WB_PaintWindow(TabletApplication &app, QWidget *parent) : QMainWindow(parent), ui(new Ui::WB_PaintWindow) {
    ui->setupUi(this);

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
    connect(&app, &TabletApplication::sendTabletActive, this, &WB_PaintWindow::tabletActive);
}

QLabel *createToolbarHeader(QString const &name) {
    auto *item = new QLabel(name);
    item->setAlignment(Qt::AlignCenter);
    item->setMargin(2);
    return item;
}

void WB_PaintWindow::setupColorActions() {
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

void WB_PaintWindow::setupPenActions() {
    ui->toolBarSettings->addWidget(createToolbarHeader("Pen"));

    auto *penSelector = new QActionGroup(this);
    penSelector->setExclusive(true);

    addPenAction(10, penSelector);
    addPenAction(5, penSelector)->trigger();
    addPenAction(1, penSelector);
}

ColorAction *WB_PaintWindow::addColorAction(const QColor &color, QActionGroup *selector) {
    auto *newAction = new ColorAction(color, this);
    connect(newAction, &ColorAction::colorSelected, ui->graphicsView, &WB_GraphicsView::setColor);
    ui->toolBarSettings->addAction(newAction);
    selector->addAction(newAction);
    return newAction;
}

PenAction *WB_PaintWindow::addPenAction(int thickness, QActionGroup *selector) {
    auto *newAction = new PenAction(thickness, this);
    connect(newAction, &PenAction::penSelected, ui->graphicsView, &WB_GraphicsView::setPenThickness);
    ui->toolBarSettings->addAction(newAction);
    selector->addAction(newAction);
    return newAction;
}

void WB_PaintWindow::showFileSaveDialog() {
    QFileDialog dialog(this, "Save Whiteboard", "", "Whiteboard (*.whb);;All Files (*)");
    dialog.setDefaultSuffix(".whb");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() == QDialog::Accepted) {
        const auto filename = dialog.selectedFiles().front();
        ui->graphicsView->saveToFile(filename);
        addFileToRecentlyUsed(filename);
    }
}

void WB_PaintWindow::showFileLoadDialog() {
    QFileDialog dialog(this, "Open Whiteboard", "", "Whiteboard (*.whb);;All Files (*)");
    dialog.setDefaultSuffix(".whb");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec() == QDialog::Accepted) {
        loadFromFile(dialog.selectedFiles().front());
    }
}

void WB_PaintWindow::loadFromFile(QString filename) {
    ui->graphicsView->loadFromFile(filename);
    addFileToRecentlyUsed(filename);
}

void WB_PaintWindow::addFileToRecentlyUsed(QString filename) {
    QSettings settings;
    auto files = settings.value("recentlyUsedFiles").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    settings.setValue("recentlyUsedFiles", QVariant::fromValue(files));
    updateRecentlyUsedFiles();
}

void WB_PaintWindow::updateRecentlyUsedFiles() {
    ui->actionOpen->menu()->clear();
    QSettings settings;
    auto const files = settings.value("recentlyUsedFiles").toStringList();
    for (auto const &i: files)
        ui->actionOpen->menu()->addAction(i, [this, i](){loadFromFile(i);});
}

bool WB_PaintWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease) {
        if (obj == ui->graphicsView->viewport()) {
            return m_tabletActive;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

WB_PaintWindow::~WB_PaintWindow() {
    delete ui;
}