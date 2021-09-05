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

#include <QTextCharFormat>
#include <QTextCursor>
#include <QUndoCommand>

#include "items/wb_text_item.h"
#include "views/wb_graphics_view.h"
#include "views/wb_graphics_scene.h"


class TextTool::AddCommand : public QUndoCommand {
public:
    explicit AddCommand(WB_TextItem *item, WB_GraphicsScene *graphicsScene, QUndoCommand *parent = nullptr) :
            QUndoCommand("Add Text", parent),
            m_item(item),
            m_scene(graphicsScene) {
        m_item->setZValue(m_scene->getNewForegroundZ());
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
    WB_GraphicsScene *m_scene;
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
    auto const pos = view.mapToScene(event.pos());

    if (WB_TextItem *item = qgraphicsitem_cast<WB_TextItem*>(view.scene()->itemAt(pos, QTransform()))) {
        item->enableEditor();
    } else {
        item = new WB_TextItem("");
        item->setFocus();

        QTextCharFormat fmt;
        fmt.setForeground(QBrush(m_color));
        QTextCursor cursor = item->textCursor();
        cursor.setCharFormat(fmt);
        item->setTextCursor(cursor);

        item->setFont(m_font);
        item->enableEditor();

        m_currentCommand = new AddCommand(item, view.scene());
        view.getUndoStack()->push(m_currentCommand);
    }
    return true;
}

void TextTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    if (m_currentCommand) {
        m_currentCommand->setPos(view.mapToScene(event.pos()));
    }
}

void TextTool::handleTabletRelease(WB_GraphicsView &, QTabletEvent &) {
    m_currentCommand = nullptr;
}
