#ifndef WHITEBOARDTOOL_H
#define WHITEBOARDTOOL_H

#include <QTabletEvent>

class WhiteBoardGraphicsView;

class WhiteBoardTool {
public:
    virtual void handleTabletPress(WhiteBoardGraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletMove(WhiteBoardGraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletRelease(WhiteBoardGraphicsView &, QTabletEvent &){};
};

#endif // WHITEBOARDTOOL_H
