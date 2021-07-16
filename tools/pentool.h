#ifndef PENTOOL_H
#define PENTOOL_H

#include <QGraphicsPathItem>
#include <QGraphicsScene>

#include "whiteboardtool.h"
#include "items/whiteboarditemgroup.h"

class PenTool : public WhiteBoardTool {
public:
    class DrawCommand;

    virtual void setColor(QColor const &color);
    virtual void setPen(int thickness);

    void handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event);

    PenTool operator=(PenTool &) = delete;
    PenTool(PenTool &) = delete;
    PenTool() = default;

protected:
    virtual WhiteBoardItemGroup *createGroup();

private:
    QColor m_color = QColor(200, 200, 200);
    int m_thickness = 10;

    DrawCommand *m_currentCommand;
    QPointF m_previous;
};

#endif // PENTOOL_H
