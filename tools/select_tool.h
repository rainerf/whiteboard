#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "tool_base.h"

#include <QGraphicsRectItem>

class SelectTool : public ToolBase {
    class MoveCommand;

public:
    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletRelease(WB_GraphicsView &view, QTabletEvent &event) override;

private:
    QPointF m_previous;
    QGraphicsRectItem m_selectionRect;
    MoveCommand *m_moveCommand;
    enum Mode {selecting, moving} mode;
};

#endif // SELECTTOOL_H
