#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include <QGraphicsItemGroup>

#include "whiteboardtool.h"

class PointerTool : public WhiteBoardTool {
public:
    void setColor(QColor const &color);

    void handleTabletPress(QGraphicsView &view, QTabletEvent &event) override;
    void handleTabletMove(QGraphicsView &view, QTabletEvent &event) override;
    void handleTabletRelease(QGraphicsView &view, QTabletEvent &) override;

    PointerTool operator=(PointerTool &) = delete;
    PointerTool(PointerTool &) = delete;
    PointerTool();
    ;

private:
    qreal const m_size = 30;
    QColor m_color = QColor(200, 0, 0, 150);
    QGraphicsEllipseItem m_pointer{-m_size / 2, -m_size / 2, m_size, m_size};
};

#endif // POINTERTOOL_H
