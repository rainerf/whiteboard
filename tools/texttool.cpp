#include "texttool.h"

#include <QUndoCommand>

#include "items/whiteboardtextitem.h"
#include "views/whiteboardgraphicsview.h"


class TextTool::AddCommand : public QUndoCommand {
public:
    explicit AddCommand(WhiteBoardTextItem *item, QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr) :
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

void TextTool::handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    auto *textItem = new WhiteBoardTextItem("");
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setPos(view.mapToScene(event.pos()));
    textItem->setFocus();
    textItem->setDefaultTextColor(m_color);
    textItem->setFont(m_font);

    m_currentCommand = new AddCommand(textItem, view.scene());
    view.getUndoStack()->push(m_currentCommand);
}

void TextTool::handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event) {
    if (m_currentCommand) {
        m_currentCommand->setPos(view.mapToScene(event.pos()));
    }
}
