#ifndef PANTOOL_H
#define PANTOOL_H

#include <QFont>

#include "whiteboardtool.h"

class PanTool : public WhiteBoardTool {
public:
    void handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event);

    PanTool operator=(PanTool &) = delete;
    PanTool(PanTool &) = delete;
    PanTool() = default;

private:
    QPointF m_previous;
};
#endif // PANTOOL_H
