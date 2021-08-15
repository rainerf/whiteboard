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

#include "highlight_tool.h"
#include "views/wb_graphics_view.h"
#include "views/wb_graphics_scene.h"


void HighlightTool::setColor(QColor const &color) {
    QColor color_ = QColor::fromHsv(color.hue(), color.saturation() / 5, color.value());
    PenTool::setColor(color_);
}

void HighlightTool::setPen(int thickness) {
    PenTool::setPen(thickness * 10);
}

qreal HighlightTool::getZ(WB_GraphicsScene *scene) {
    return scene->getNewBackgroundZ();
}
