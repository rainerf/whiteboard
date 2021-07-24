// This file is part of Whiteboard.
//
// Whiteboard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Whiteboard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Whiteboard.  If not, see <https://www.gnu.org/licenses/>.

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

    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletRelease(WB_GraphicsView &view, QTabletEvent &event) override;

    PenTool operator=(PenTool &) = delete;
    PenTool(PenTool &) = delete;
    PenTool() = default;

protected:
    virtual WB_ItemGroup *createGroup();

private:
    QColor m_color = QColor(200, 200, 200);
    int m_maxThickness = 10;

    DrawCommand *m_currentCommand;
};

#endif // PENTOOL_H
