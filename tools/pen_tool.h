#ifndef PENTOOL_H
#define PENTOOL_H

#include <QGraphicsPathItem>
#include <QGraphicsScene>

#include "tool_base.h"
#include "items/wb_item_group.h"

class PenTool : public ToolBase {
public:
    class DrawCommand;

    virtual void setColor(QColor const &color);
    virtual void setPen(int thickness);

    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event);

    PenTool operator=(PenTool &) = delete;
    PenTool(PenTool &) = delete;
    PenTool() = default;

protected:
    virtual WB_ItemGroup *createGroup();

private:
    QColor m_color = QColor(200, 200, 200);
    int m_thickness = 10;

    DrawCommand *m_currentCommand;
    QPointF m_previous;
};

#endif // PENTOOL_H
