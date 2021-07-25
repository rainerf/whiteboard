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

#include "text_tool.h"

#include <QUndoCommand>

#include "items/wb_text_item.h"
#include "views/wb_graphics_view.h"


class TextTool::AddCommand : public QUndoCommand {
public:
    explicit AddCommand(WB_TextItem *item, QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr) :
            QUndoCommand("Add Text", parent),
            m_item(item),
            m_scene(graphicsScene) {
    }

    void undo() override {
        m_scene->removeItem(m_item);
    }

    void redo() override {
        m_scene->addItem(m_item);
        m_scene->clearSelection();
        m_item->setSelected(true);
    }

    void setPos(QPointF const &pos) {
        m_item->setPos(pos);
    }

private:
    QGraphicsTextItem *m_item;
    QGraphicsScene *m_scene;
};


void TextTool::setColor(const QColor &color) {
    m_color = color;
}

void TextTool::setFont(const QFont &font) {
    m_font = font;
    m_font.setPixelSize(m_fontSize);
}

void TextTool::setFontSize(int size) {
    m_fontSize = size;
    m_font.setPixelSize(m_fontSize);
}

bool TextTool::handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) {
    auto *textItem = new WB_TextItem("");
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setPos(view.mapToScene(event.pos()));
    textItem->setFocus();
    textItem->setDefaultTextColor(m_color);
    textItem->setFont(m_font);

    m_currentCommand = new AddCommand(textItem, view.scene());
    view.getUndoStack()->push(m_currentCommand);
    return true;
}

void TextTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    if (m_currentCommand) {
        m_currentCommand->setPos(view.mapToScene(event.pos()));
    }
}
