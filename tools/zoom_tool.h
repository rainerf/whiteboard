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

#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H

#include <QFont>

#include "tool_base.h"

class ZoomTool : public ToolBase {
public:
    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) override;
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) override;

    ZoomTool operator=(ZoomTool &) = delete;
    ZoomTool(ZoomTool &) = delete;
    ZoomTool() = default;

private:
    QPointF m_previous;
};
#endif // ZOOMTOOL_H
