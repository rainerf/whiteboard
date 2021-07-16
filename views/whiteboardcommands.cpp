#include "whiteboardcommands.h"

#include <QApplication>
#include <QMimeData>
#include <QClipboard>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "items/whiteboardpixmapitem.h"
#include "items/whiteboardtextitem.h"
#include "lib/qgraphicsscenestorage.h"
#include "whiteboardcopypastesupport.h"

PasteCommand::PasteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent) : QUndoCommand("Paste", parent), m_scene(graphicsScene) {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->formats().contains(MIME_TYPE)) {
        m_items = pasteFromMimeData(mimeData);
    } else if (mimeData->hasImage()) {
        m_items.append(new WhiteBoardPixmapItem(qvariant_cast<QPixmap>(mimeData->imageData())));
    } else if (mimeData->hasHtml()) {
        auto *textItem = new WhiteBoardTextItem();
        textItem->setHtml(mimeData->html());
        m_items.append(textItem);
    } else if (mimeData->hasText()) {
        auto *textItem = new WhiteBoardTextItem();
        textItem->setPlainText(mimeData->text());
        m_items.append(textItem);
    }
}

void PasteCommand::undo() {
    for (auto &&i: m_items)
        m_scene->removeItem(i);
}

void PasteCommand::redo() {
    m_scene->clearSelection();
    for (auto &&i: m_items) {
        m_scene->addItem(i);
        i->setSelected(true);
    }
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
