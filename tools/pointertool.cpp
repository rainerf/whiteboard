#include "pointertool.h"

void PointerTool::setColor(const QColor &color) {
    m_color = color;
}

void PointerTool::handleTabletPress(QGraphicsView &view, QTabletEvent &event) {
    m_pointer.setPos(view.mapToScene(event.pos()));
    view.scene()->addItem(&m_pointer);
}

void PointerTool::handleTabletMove(QGraphicsView &view, QTabletEvent &event) {
    m_pointer.setPos(view.mapToScene(event.pos()));
}

void PointerTool::handleTabletRelease(QGraphicsView &view, QTabletEvent &) {
    view.scene()->removeItem(&m_pointer);
}

PointerTool::PointerTool() {
    m_pointer.setBrush(QBrush(m_color));
    m_pointer.setPen(QPen(m_color));
}
