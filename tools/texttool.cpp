#include "texttool.h"
#include "items/whiteboardtextitem.h"
#include "views/whiteboardgraphicsview.h"

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

void TextTool::handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    m_textItem = new WhiteBoardTextItem("");
    m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_textItem->setPos(view.mapToScene(event.pos()));
    m_textItem->setFocus();
    m_textItem->setDefaultTextColor(m_color);
    m_textItem->setFont(m_font);
    view.scene()->addItem(m_textItem);
    view.scene()->clearSelection();
    m_textItem->setSelected(true);
}

void TextTool::handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    if (m_textItem) {
        m_textItem->setPos(view.mapToScene(event.pos()));
        m_textItem->setFocus();
    }
}
