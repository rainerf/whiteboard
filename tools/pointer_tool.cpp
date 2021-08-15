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

#include "pointer_tool.h"
#include "views/wb_graphics_view.h"
#include "views/wb_graphics_scene.h"


void PointerTool::setColor(const QColor &color) {
    m_color = color;
}

bool PointerTool::handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) {
    m_pointer.setPos(view.mapToScene(event.pos()));
    m_pointer.setFlags(QGraphicsItem::ItemIgnoresTransformations);
    view.scene()->addItem(&m_pointer);
    return true;
}

void PointerTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    m_pointer.setPos(view.mapToScene(event.pos()));
}

void PointerTool::handleTabletRelease(WB_GraphicsView &view, QTabletEvent &) {
    view.scene()->removeItem(&m_pointer);
}

PointerTool::PointerTool() {
    m_pointer.setBrush(QBrush(m_color));
    m_pointer.setPen(QPen(m_color));
    m_pointer.setZValue(WB_GraphicsScene::getAbsoluteForegroundZ());
}
