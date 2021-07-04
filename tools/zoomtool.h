#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H

#include <QFont>

#include "whiteboardtool.h"

class ZoomTool : public WhiteBoardTool {
public:
    void handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event);

    ZoomTool operator=(ZoomTool &) = delete;
    ZoomTool(ZoomTool &) = delete;
    ZoomTool() = default;

private:
    QPointF m_previous;
};
#endif // ZOOMTOOL_H
