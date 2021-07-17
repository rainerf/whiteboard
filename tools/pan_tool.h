#ifndef PANTOOL_H
#define PANTOOL_H

#include <QFont>

#include "tool_base.h"

class PanTool : public ToolBase {
public:
    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event);

    PanTool operator=(PanTool &) = delete;
    PanTool(PanTool &) = delete;
    PanTool() = default;

private:
    QPointF m_previous;
};
#endif // PANTOOL_H
