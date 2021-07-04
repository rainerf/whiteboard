#include "pentool.h"

void PenTool::setColor(const QColor &color) {
    m_color = color;
}

void PenTool::setPen(int thickness) {
    m_thickness = thickness;
}

void PenTool::handleTabletPress(QGraphicsView &view, QTabletEvent &event) {
    m_group = new QGraphicsItemGroup();
    m_group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    view.scene()->addItem(m_group);

    m_previous = view.mapToScene(event.pos());
}

void PenTool::handleTabletMove(QGraphicsView &view, QTabletEvent &event) {
    if (m_group) {
        auto current = view.mapToScene(event.pos());
        if (current == m_previous)
            return;

        QPainterPath p;
        p.moveTo(m_previous);
        p.lineTo(current);

        m_path = new QGraphicsPathItem();
        m_path->setPen(QPen(m_color, event.pressure() * m_thickness*3/4 + m_thickness/4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_path->setPath(p);
        m_group->addToGroup(m_path);

        m_previous = current;
    }
}

QGraphicsItemGroup *PenTool::currentGroup() {
    return m_group;
}
