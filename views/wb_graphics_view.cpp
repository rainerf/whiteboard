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

#include "wb_graphics_view.h"

#include <QGraphicsView>
#include <QTabletEvent>
#include <QMessageBox>

#include "commands.h"
#include "copy_paste_support.h"
#include "wb_graphics_scene.h"

void WB_GraphicsView::tabletEvent(QTabletEvent *event) {
    switch (event->type()) {
    case QEvent::TabletPress:
        // take the focus from other widgets (which Qt doesn't do for some reason),
        // and unfocus all items in the scene
        setFocus();
        for (auto &&i : scene()->topLevelItems())
            i->clearFocus();

        if (!m_deviceDown) {
            m_deviceDown = true;
            // handle the event; if the function returns true, this means to clear
            // all focused items
            if (currentTool->handleTabletPress(*this, *event))
                for (auto &&i : scene()->topLevelItems())
                    i->setSelected(false);
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

WB_GraphicsView::WB_GraphicsView(QWidget *parent): InteractiveView(parent), m_scene(new WB_GraphicsScene(this)) {
    setCursor(Qt::CrossCursor);
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

void WB_GraphicsView::setSelectTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &selectTool;
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

void WB_GraphicsView::debugDumpAllItems() {
    m_scene->debugDumpAllItems();
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

void WB_GraphicsView::cut() {
    copy();
    deleteSelectedItems();
}

void WB_GraphicsView::selectAll() {
    for (auto &&i : scene()->topLevelItems())
        i->setSelected(true);
}

void WB_GraphicsView::saveToFile(QString const &filename) {
    m_scene->saveToFile(filename);
}

void WB_GraphicsView::loadFromFile(QString const &filename) {
    try {
        m_scene->loadFromFile(filename);
        setZoom(1);
        centerOn(0, 0);
    } catch (...) {
        QMessageBox::critical(this, "Whiteboard", "File format incorrect!");
    }
}

void WB_GraphicsView::clear() {
    selectAll();
    deleteSelectedItems();
}
