#ifndef WHITEBOARDTOOL_H
#define WHITEBOARDTOOL_H

#include <QTabletEvent>

class WB_GraphicsView;

class ToolBase {
public:
    virtual void handleTabletPress(WB_GraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletMove(WB_GraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletRelease(WB_GraphicsView &, QTabletEvent &){};
};

#endif // WHITEBOARDTOOL_H
