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

#include "paste_command.h"
#include "items/wb_pixmap_item.h"
#include "items/wb_text_item.h"
#include "items/wb_svg_item.h"
#include "lib/qgraphicsscene_storage.h"
#include "import_export_support.h"
#include "core/wb_graphics_scene.h"

#include <QApplication>
#include <QMimeData>
#include <QClipboard>
#include <QGraphicsItem>
#include <QMimeDatabase>

PasteCommand::PasteCommand(WB_GraphicsScene *graphicsScene, QUndoCommand *parent) : QUndoCommand("Paste", parent), m_scene(graphicsScene) {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->formats().contains(MIME_TYPE)) {
        m_items = pasteFromMimeData(mimeData);
    } else if (mimeData->hasUrls()) {
        for (auto &&url: mimeData->urls()) {
            if (url.isLocalFile() && QMimeDatabase().mimeTypeForFile(url.toLocalFile()).name().contains("svg+xml")) {
                m_items.append(new WB_SvgItem(url.toLocalFile()));
            }
        }
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

    if (m_items.empty())
        throw NothingToPasteError();

    // ensure that items pasted from our mime data are in the correct z order
    std::sort(std::begin(m_items), std::end(m_items), [](QGraphicsItem *a, QGraphicsItem *b) { return a->zValue() < b->zValue(); });

    for (auto &&i: m_items) {
        if (i->parentItem())
            continue;

        if (i->zValue() >= 0)
            i->setZValue(graphicsScene->getNewForegroundZ());
        else
            i->setZValue(graphicsScene->getNewBackgroundZ());
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
}
