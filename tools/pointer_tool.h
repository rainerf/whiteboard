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

#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include <QColor>
#include <QGraphicsEllipseItem>

#include "tool_base.h"

class PointerTool : public ToolBase {
public:
    void setColor(QColor const &color);

    bool handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletRelease(WB_GraphicsView &view, QTabletEvent &) override;

    PointerTool operator=(PointerTool &) = delete;
    PointerTool(PointerTool &) = delete;
    PointerTool();

private:
    qreal const m_size = 30;
    QColor m_color = QColor(200, 0, 0, 150);
    QGraphicsEllipseItem m_pointer{-m_size / 2, -m_size / 2, m_size, m_size};
};

#endif // POINTERTOOL_H
