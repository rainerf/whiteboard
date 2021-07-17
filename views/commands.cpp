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

#include "commands.h"

#include <QApplication>
#include <QMimeData>
#include <QClipboard>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "items/wb_pixmap_item.h"
#include "items/wb_text_item.h"
#include "lib/qgraphicsscene_storage.h"
#include "copy_paste_support.h"

PasteCommand::PasteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent) : QUndoCommand("Paste", parent), m_scene(graphicsScene) {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->formats().contains(MIME_TYPE)) {
        m_items = pasteFromMimeData(mimeData);
    } else if (mimeData->hasImage()) {
        m_items.append(new WB_PixmapItem(qvariant_cast<QPixmap>(mimeData->imageData())));
    } else if (mimeData->hasHtml()) {
        auto *textItem = new WB_TextItem();
        textItem->setHtml(mimeData->html());
        m_items.append(textItem);
    } else if (mimeData->hasText()) {
        auto *textItem = new WB_TextItem();
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
