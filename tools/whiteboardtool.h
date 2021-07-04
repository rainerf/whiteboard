#ifndef WHITEBOARDTOOL_H
#define WHITEBOARDTOOL_H

#include <QGraphicsView>
#include <QTabletEvent>

class WhiteBoardTool {
public:
    virtual void handleTabletPress(QGraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletMove(QGraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletRelease(QGraphicsView &, QTabletEvent &){};
};

#endif // WHITEBOARDTOOL_H
