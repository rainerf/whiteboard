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

#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QFont>
#include <QColor>

#include "tool_base.h"
#include "items/wb_text_item.h"

class TextTool : public ToolBase {
public:
    class AddCommand;

    void setColor(QColor const &color);
    void setFont(QFont const &font);
    void setFontSize(int size);

    void handleTabletPress(WB_GraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WB_GraphicsView &view, QTabletEvent &event);

    TextTool operator=(TextTool &) = delete;
    TextTool(TextTool &) = delete;
    TextTool() = default;

private:
    AddCommand *m_currentCommand;

    QColor m_color = QColor(200, 200, 200);
    QFont m_font;
    int m_fontSize = 8;
};
#endif // TEXTTOOL_H
