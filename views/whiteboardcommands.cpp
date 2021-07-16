#include "whiteboardcommands.h"

#include <QApplication>
#include <QMimeData>
#include <QClipboard>

#include "items/whiteboardpixmapitem.h"
#include "items/whiteboardtextitem.h"

PasteCommand::PasteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent) : QUndoCommand("Paste", parent), m_scene(graphicsScene) {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasImage()) {
        m_item = new WhiteBoardPixmapItem(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        auto *textItem = new WhiteBoardTextItem();
        textItem->setHtml(mimeData->html());
        m_item = textItem;
    } else if (mimeData->hasText()) {
        auto *textItem = new WhiteBoardTextItem();
        textItem->setPlainText(mimeData->text());
        m_item = textItem;
    }
}

void PasteCommand::undo() {
    m_scene->removeItem(m_item);
}

void PasteCommand::redo() {
    m_scene->addItem(m_item);
    m_scene->update();
}

DeleteCommand::DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent) : QUndoCommand("Delete", parent), m_scene(graphicsScene) {
    m_items = graphicsScene->selectedItems();
}

void DeleteCommand::undo() {
    for (auto &&i: m_items) {
        m_scene->addItem(i);
    }
    m_scene->update();
}

void DeleteCommand::redo() {
    for (auto &&i : m_items) {
        m_scene->removeItem(i);
    }
}
