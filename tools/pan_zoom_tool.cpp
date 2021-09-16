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

#include "pan_zoom_tool.h"
#include "core/wb_graphics_view.h"

bool PanZoomTool::handleTabletPress(WB_GraphicsView &, QTabletEvent &event) {
    m_previous = event.pos();
    return false;
}

void PanZoomTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    QPoint const current = event.pos();

    if (event.modifiers() & Qt::ControlModifier) {
        // zooming
        if (current.y() > m_previous.y() + 10) {
            view.zoomOut();
            m_previous = current;
        } else if (current.y() < m_previous.y() - 10) {
            view.zoomIn();
            m_previous = current;
        }
    } else {
        // panning
        view.pan(current - m_previous);
        m_previous = current;
    }
}
