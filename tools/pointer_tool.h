#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include <QColor>
#include <QGraphicsEllipseItem>

#include "tool_base.h"

class PointerTool : public ToolBase {
public:
    void setColor(QColor const &color);

    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletRelease(WB_GraphicsView &view, QTabletEvent &) override;

    PointerTool operator=(PointerTool &) = delete;
    PointerTool(PointerTool &) = delete;
    PointerTool();

private:
    qreal const m_size = 30;
    QColor m_color = QColor(200, 0, 0, 150);
    QGraphicsEllipseItem m_pointer{-m_size / 2, -m_size / 2, m_size, m_size};
};

#endif // POINTERTOOL_H
