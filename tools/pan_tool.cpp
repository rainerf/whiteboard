#include "zoom_tool.h"
#include "views/wb_graphics_view.h"

void PanTool::handleTabletPress(WB_GraphicsView &, QTabletEvent &event) {
    m_previous = event.pos();
}

void PanTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    QPointF current = (event.pos());
    view.pan(current - m_previous, false);

    m_previous = current;
}
