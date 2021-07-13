#include "zoomtool.h"
#include "views/whiteboardgraphicsview.h"

void PanTool::handleTabletPress(WhiteBoardGraphicsView &, QTabletEvent &event) {
    m_previous = event.pos();
}

void PanTool::handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    QPointF current = (event.pos());
    view.pan(current - m_previous, false);

    m_previous = current;
}
