#include "pointertool.h"
#include "views/whiteboardgraphicsview.h"


void PointerTool::setColor(const QColor &color) {
    m_color = color;
}

void PointerTool::handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    m_pointer.setPos(view.mapToScene(event.pos()));
    m_pointer.setFlags(QGraphicsItem::ItemIgnoresTransformations);
    view.scene()->addItem(&m_pointer);
}

void PointerTool::handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    m_pointer.setPos(view.mapToScene(event.pos()));
}

void PointerTool::handleTabletRelease(WhiteBoardGraphicsView &view, QTabletEvent &) {
    view.scene()->removeItem(&m_pointer);
}

PointerTool::PointerTool() {
    m_pointer.setBrush(QBrush(m_color));
    m_pointer.setPen(QPen(m_color));
}
