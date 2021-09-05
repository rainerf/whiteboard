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
#include <QMessageBox>
#include <QSettings>
#include <QShortcut>
//#include <QGLWidget>

#include "actions/color_action.h"
#include "actions/pen_action.h"
#include "ui_wb_paint_window.h"


namespace detail {
constexpr char STATE_SETTING[] = "paintWindow/windowState";
}


void WB_PaintWindow::setupToolSelectors() {
    auto *toolSelector = new QActionGroup(this);
    toolSelector->setExclusive(true);
    toolSelector->addAction(ui->actionText);
    toolSelector->addAction(ui->actionHighlight);
    toolSelector->addAction(ui->actionPointer);
    toolSelector->addAction(ui->actionPen);
    toolSelector->addAction(ui->actionPanZoomTool);
    toolSelector->addAction(ui->actionSelectMove);

    connect(ui->actionPen, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setPenTool);
    connect(ui->actionText, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setTextTool);
    connect(ui->actionHighlight, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setHighlightTool);
    connect(ui->actionPointer, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setPointerTool);
    connect(ui->actionPanZoomTool, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setPanZoomTool);
    connect(ui->actionSelectMove, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setSelectTool);

    // disable changing the tool while one is in use
    connect(ui->graphicsView, &WB_GraphicsView::toolInUse, toolSelector, &QActionGroup::setDisabled);
}

void WB_PaintWindow::setupUiActions() {
    connect(ui->actionCopy, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::copy);
    connect(ui->actionCut, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::cut);
    connect(ui->actionPaste, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::selectAll);
    connect(ui->actionSave, &QAction::triggered, this, &WB_PaintWindow::showFileSaveDialog);
    connect(ui->actionOpen, &QAction::triggered, this, &WB_PaintWindow::showFileLoadDialog);
    ui->actionOpen->setMenu(new QMenu()); // recently used menu
    updateRecentlyUsedFiles();
    connect(ui->actionClear, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::clear);
    connect(ui->actionZoomOriginal, &QAction::triggered, [=]() { ui->graphicsView->setZoom(1); });
    connect(ui->actionZoomFit, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::zoomToFit);
    connect(ui->actionGrid, &QAction::triggered, ui->graphicsView, &WB_GraphicsView::setGrid);

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

    auto *dumpShortcut = new QShortcut(QKeySequence("Ctrl+Shift+D"), this);
    connect(dumpShortcut, &QShortcut::activated, ui->graphicsView, &WB_GraphicsView::debugDumpAllItems);
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

    connect(ui->graphicsView, &WB_GraphicsView::fileModified, this, &WB_PaintWindow::setFileModified);

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

    QSettings settings;
    restoreState(settings.value(detail::STATE_SETTING).toByteArray());
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

bool WB_PaintWindow::showFileSaveDialog() {
    QFileDialog dialog(this, "Save Whiteboard", "", "Whiteboard (*.whb);;All Files (*)");
    dialog.setDefaultSuffix(".whb");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() == QDialog::Accepted) {
        const auto filename = dialog.selectedFiles().front();
        ui->graphicsView->saveToFile(filename);
        addFileToRecentlyUsed(filename);
        return true;
    }

    return false;
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

void WB_PaintWindow::closeEvent(QCloseEvent *event) {
    if (m_fileModified) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch (msgBox.exec()) {
        case QMessageBox::Save:
            if (showFileSaveDialog())
                break; // file was saved, we're good
            else {
                event->ignore();
                return;
            }
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        }
    }

    QSettings settings;
    settings.setValue(detail::STATE_SETTING, saveState());
    QMainWindow::closeEvent(event);
}

void WB_PaintWindow::setFileModified(bool x) {
    m_fileModified = x;
    emit fileModified(m_fileModified);
}

WB_PaintWindow::~WB_PaintWindow() {
    delete ui;
}
