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

    m_path = new QGraphicsPathItem();
    m_previous = view.mapToScene(event.pos());
    QPainterPath p;
    p.moveTo(m_previous);
    m_path->setPath(p);
    m_group->addToGroup(m_path);
    view.scene()->addItem(m_group);
}

void PenTool::handleTabletMove(QGraphicsView &view, QTabletEvent &event) {
    if (m_path) {
        QPainterPath path = m_path->path();
        m_previous = view.mapToScene(event.pos());
        path.lineTo(m_previous);
        m_path->setPath(path);

        m_path = new QGraphicsPathItem();
        m_path->setPen(QPen(m_color, event.pressure() * m_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        QPainterPath p;
        p.moveTo(m_previous);
        m_path->setPath(p);
        m_group->addToGroup(m_path);
    }
}

QGraphicsItemGroup *PenTool::currentGroup() {
    return m_group;
}
