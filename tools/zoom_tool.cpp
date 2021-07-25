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

#include "zoom_tool.h"
#include "views/wb_graphics_view.h"

bool ZoomTool::handleTabletPress(WB_GraphicsView &, QTabletEvent &event) {
    // NB: we're deliberately not mapping the event's position to the scene here:
    // we'll be zooming the scene, therefore we'll be changing the scene coordinates
    // the widget's point maps to. Also, the zooming should not depend on the current
    // zoom factor, which it would if we're mapping to the scene's coordinates.
    m_previous = event.pos();
    return false;
}

void ZoomTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    QPointF current = event.pos();
    if (current.y() > m_previous.y() + 10) {
        view.zoomOut();
        m_previous = current;
    } else if (current.y() < m_previous.y() - 10) {
        view.zoomIn();
        m_previous = current;
    }
}
