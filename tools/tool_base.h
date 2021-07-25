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

#ifndef WHITEBOARDTOOL_H
#define WHITEBOARDTOOL_H

#include <QTabletEvent>

class WB_GraphicsView;

class ToolBase {
public:
    // handle the tablet press event; return true if, after the press, all
    // selections should be cleared
    virtual bool handleTabletPress(WB_GraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletMove(WB_GraphicsView &, QTabletEvent &) = 0;
    virtual void handleTabletRelease(WB_GraphicsView &, QTabletEvent &){};
};

#endif // WHITEBOARDTOOL_H
