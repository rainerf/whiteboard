#include "zoomtool.h"
#include "views/whiteboardgraphicsview.h"

void ZoomTool::handleTabletPress(WhiteBoardGraphicsView &, QTabletEvent &event) {
    // NB: we're deliberately not mapping the event's position to the scene here:
    // we'll be zooming the scene, therefore we'll be changing the scene coordinates
    // the widget's point maps to. Also, the zooming should not depend on the current
    // zoom factor, which it would if we're mapping to the scene's coordinates.
    m_previous = event.pos();
}

void ZoomTool::handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    QPointF current = event.pos();
    if (current.y() > m_previous.y() + 10) {
        view.zoomOut();
        m_previous = current;
    } else if (current.y() < m_previous.y() - 10) {
        view.zoomIn();
        m_previous = current;
    }
}
