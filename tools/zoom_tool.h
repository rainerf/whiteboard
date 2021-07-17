#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H

#include <QFont>

#include "tool_base.h"

class ZoomTool : public ToolBase {
public:
    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event);

    ZoomTool operator=(ZoomTool &) = delete;
    ZoomTool(ZoomTool &) = delete;
    ZoomTool() = default;

private:
    QPointF m_previous;
};
#endif // ZOOMTOOL_H
