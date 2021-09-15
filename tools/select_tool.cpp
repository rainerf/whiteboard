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

#include "select_tool.h"

#include "views/wb_graphics_view.h"
#include "views/wb_graphics_scene.h"

#include <QUndoCommand>

class SelectTool::MoveCommand : public QUndoCommand {
public:
    explicit MoveCommand(QList<QGraphicsItem*> items, QPointF initial, QUndoCommand *parent = nullptr) :
        QUndoCommand("Move", parent),
        m_items(items),
        m_initial(initial),
        m_final(initial) {
    }

    void undo() override {
        moveAllBy(m_initial - m_final);
    }

    void redo() override {
        moveAllBy(m_final - m_initial);
    }

    void moveBy(QPointF const &delta) {
        moveAllBy(delta);
        m_final += delta;
    }

private:
    void moveAllBy(QPointF const &delta) {
        for (auto &&i: m_items) {
            i->moveBy(delta.x(), delta.y());
        }
    }

    QList<QGraphicsItem*> m_items;
    QPointF m_initial;
    QPointF m_final;
};

bool SelectTool::handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) {
    m_first = view.mapToScene(event.pos());
    auto *item = view.scene()->itemAt(m_first, view.transform());
    if (item && item->isSelected()) {
        // if the item is part of a group, we'll always use the group
        if (item->parentItem())
            item = item->parentItem();

        mode = moving;
        // if some items were selected, but not this one, remove the previous selection
        // and only select this one (below)
        if (!view.scene()->selectedItems().contains(item))
            view.scene()->clearSelection();
        if (view.scene()->selectedItems().isEmpty())
            item->setSelected(true);

        m_moveCommand = new MoveCommand(view.scene()->selectedItems(), m_first);
        view.getUndoStack()->push(m_moveCommand);
    } else {
        mode = selecting;

        m_selectionRect.setRect(m_first.x(), m_first.y(), 1, 1);
        view.scene()->addItem(&m_selectionRect);
    }
    return false;
}

void SelectTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    QPointF const current = view.mapToScene(event.pos());
    if (mode == moving) {
        QPointF const delta = current - m_first;
        m_moveCommand->moveBy(delta);
        m_first = current;
    } else {
        QRectF rect = QRectF{m_first, current}.normalized();
        m_selectionRect.setRect(rect);

        QPainterPath selectionArea;
        selectionArea.addPolygon(rect);
        selectionArea.closeSubpath();

        view.scene()->setSelectionArea(selectionArea, Qt::ReplaceSelection, Qt::ContainsItemShape);
    }
}

void SelectTool::handleTabletRelease(WB_GraphicsView &view, QTabletEvent & event) {
    if (mode == moving) {
        m_moveCommand = nullptr;
    } else {
        view.scene()->removeItem(&m_selectionRect);

        // if first was on an item, nothing was selected, and the selection rectangle
        // was very small, let's assume that the user "clicked" the item to select it
        auto *item = view.scene()->itemAt(m_first, view.transform());
        if (item &&
                view.scene()->selectedItems().empty() &&
                (view.mapFromScene(m_first) - event.pos()).manhattanLength() < 5) {
            item->setSelected(true);
        }
    }
}

SelectTool::SelectTool() {
    QPen pen(QColor(0, 0, 255));
    pen.setCosmetic(true);
    m_selectionRect.setPen(pen);
    m_selectionRect.setBrush(QBrush(QColor(0, 0, 255, 40)));
    m_selectionRect.setZValue(WB_GraphicsScene::getAbsoluteForegroundZ());
}
