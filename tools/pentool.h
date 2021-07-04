#ifndef PENTOOL_H
#define PENTOOL_H

#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>

#include "whiteboardtool.h"

class PenTool : public WhiteBoardTool {
public:
    virtual void setColor(QColor const &color);
    virtual void setPen(int thickness);

    void handleTabletPress(QGraphicsView &view, QTabletEvent &event);
    void handleTabletMove(QGraphicsView &view, QTabletEvent &event);

    PenTool operator=(PenTool &) = delete;
    PenTool(PenTool &) = delete;
    PenTool() = default;

protected:
    QGraphicsItemGroup *currentGroup();

private:
    QColor m_color = QColor(200, 200, 200);
    int m_thickness = 10;

    QGraphicsPathItem *m_path;
    QGraphicsItemGroup *m_group;
    QPointF m_previous;
};

#endif // PENTOOL_H
