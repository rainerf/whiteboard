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

#include "copy_paste_support.h"

#include <QList>
#include <QImage>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtMath>
#include <QPainter>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>

#include "lib/qgraphicsscene_storage.h"

QList<QGraphicsItem*> pasteFromBinary(QByteArray &itemData) {
    QDataStream inData(&itemData, QIODevice::ReadOnly);
    return readItems(inData);
}

QList<QGraphicsItem*> pasteFromMimeData(QMimeData const *mimeData) {
    auto binData = mimeData->data(MIME_TYPE);
    return pasteFromBinary(binData);
}

namespace detail {
// This is quite an ugly hack: We're pasting all elements (that were streamed
// into itemData before) into a temporary QGraphicsScene, which we can then
// just render as a whole. The former part is ugly, because we're relying on
// the data having been prepared to be copied to the clipboard already. The
// latter part is nice, though: There's no easy way to render just a few
// items, but QGraphicsScene::render() allows us to render all with a single
// call.
QImage renderToPixmap(QByteArray &itemData) {
    QGraphicsScene s;
    for (auto &&i: pasteFromBinary(itemData))
        s.addItem(i);
    s.clearSelection();
    s.update();

    int const oversampling = 8;

    auto const boundingRect = s.itemsBoundingRect();
    int width = qCeil(boundingRect.width());
    int height = qCeil(boundingRect.height());
    QImage image(width * oversampling, height * oversampling, QImage::Format_ARGB32);
    image.setDotsPerMeterX(image.dotsPerMeterX()*oversampling);
    image.setDotsPerMeterY(image.dotsPerMeterY()*oversampling);
    QPainter painter(&image);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    s.render(&painter, image.rect(), boundingRect);
    return image;
}
}

void copyGraphicsItems(QList<QGraphicsItem *> items) {
    if (items.isEmpty())
        return;

    QByteArray itemData;
    QDataStream outData(&itemData, QIODevice::WriteOnly);
    saveItems(items, outData);
    QMimeData* mimeData = new QMimeData;
    mimeData->removeFormat(MIME_TYPE);
    mimeData->setData(MIME_TYPE, itemData);
    mimeData->setImageData(detail::renderToPixmap(itemData));

    QApplication::clipboard()->setMimeData(mimeData);
}
