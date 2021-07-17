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

#include "pen_tool.h"

#include <QUndoCommand>

#include "views/wb_graphics_view.h"

class PenTool::DrawCommand : public QUndoCommand {
public:
    explicit DrawCommand(WB_ItemGroup *group, QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr) :
            QUndoCommand("Draw", parent),
            m_group(group),
            m_scene(graphicsScene) {
    }

    void undo() override {
        m_scene->removeItem(m_group);
    }

    void redo() override {
        m_scene->addItem(m_group);
    }

    void addToGroup(QGraphicsPathItem *path) {
        m_group->addToGroup(path);
    }

private:
    WB_ItemGroup *m_group;
    QGraphicsScene *m_scene;
};

void PenTool::setColor(const QColor &color) {
    m_color = color;
}

void PenTool::setPen(int thickness) {
    m_thickness = thickness;
}

void PenTool::handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) {
    m_currentCommand = new DrawCommand(createGroup(), view.scene());
    view.getUndoStack()->push(m_currentCommand);
    m_previous = view.mapToScene(event.pos());
}

void PenTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    if (m_currentCommand) {
        auto current = view.mapToScene(event.pos());
        if (current == m_previous)
            return;

        QPainterPath p;
        p.moveTo(m_previous);
        p.lineTo(current);

        auto *path = new QGraphicsPathItem();
        path->setPen(QPen(m_color, event.pressure() * m_thickness*3/4 + m_thickness/4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        path->setPath(p);
        m_currentCommand->addToGroup(path);

        m_previous = current;
    }
}

WB_ItemGroup *PenTool::createGroup() {
    auto *group = new WB_ItemGroup();
    group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    return group;
}
