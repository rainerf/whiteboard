#include "wb_graphics_view.h"

#include <QGraphicsView>
#include <QTabletEvent>
#include <QMessageBox>

#include "commands.h"
#include "copy_paste_support.h"
#include "lib/qgraphicsscene_storage.h"

void WB_GraphicsView::tabletEvent(QTabletEvent *event) {
    switch (event->type()) {
    case QEvent::TabletPress:
        // take the focus from other widgets (which Qt doesn't do for some reason),
        // and unfocus all items in the scene
        setFocus();
        for (auto &&i : scene()->items())
            i->clearFocus();

        if (!m_deviceDown) {
            m_deviceDown = true;
            currentTool->handleTabletPress(*this, *event);
            emit toolInUse(true);
        }
        break;
    case QEvent::TabletMove:
        if (m_deviceDown) {
            currentTool->handleTabletMove(*this, *event);
        }
        break;
    case QEvent::TabletRelease:
        if (m_deviceDown && event->buttons() == Qt::NoButton) {
            m_deviceDown = false;
            currentTool->handleTabletRelease(*this, *event);
            emit toolInUse(false);
        }
        break;
    default:
        break;
    }
    event->accept();
}

void WB_GraphicsView::setColor(const QColor &color) {
    penTool.setColor(color);
    textTool.setColor(color);
    highlightTool.setColor(color);
}

void WB_GraphicsView::setPenThickness(int thickness) {
    penTool.setPen(thickness);
    highlightTool.setPen(thickness);
}

void WB_GraphicsView::setPenTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &penTool;
}

void WB_GraphicsView::setTextTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &textTool;
}

void WB_GraphicsView::setHighlightTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &highlightTool;
}

void WB_GraphicsView::setPointerTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &pointerTool;
}

void WB_GraphicsView::setZoomTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &zoomTool;
}

void WB_GraphicsView::setPanTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &panTool;
}

void WB_GraphicsView::deleteSelectedItems() {
    undoStack.push(new DeleteCommand(scene()));
}

void WB_GraphicsView::setFont(const QFont &font) {
    textTool.setFont(font);
}

void WB_GraphicsView::setFontSize(int size) {
    textTool.setFontSize(size);
}

void WB_GraphicsView::paste() {
    undoStack.push(new PasteCommand(scene()));
}

void WB_GraphicsView::copy() {
    copyGraphicsItems(scene()->selectedItems());
}

void WB_GraphicsView::selectAll() {
    for (auto &&i : scene()->items())
        i->setSelected(true);
}

void WB_GraphicsView::saveToFile(QString const &filename) {
    QFile fileOut(filename);
    if (fileOut.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileOut);
        saveItems(scene()->items(), out);
        fileOut.close();
    }
}

void WB_GraphicsView::loadFromFile(QString const &filename) {
    QFile fileIn(filename);
    if (fileIn.open(QIODevice::ReadOnly)) {
        scene()->clear();

        QDataStream in(&fileIn);
        try {
            QList<QGraphicsItem *> items = readItems(in);
            for (QGraphicsItem *item : items) {
                scene()->addItem(item);
            }
        } catch (FileError &) {
            QMessageBox::critical(this, "Whiteboard", "File format incorrect!");
        }
    }
}

void WB_GraphicsView::clear() {
    scene()->clear();
}
