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

#include "import_export_support.h"

#include <QList>
#include <QImage>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtMath>
#include <QPainter>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QSvgGenerator>

#include "lib/qgraphicsscene_storage.h"

namespace detail {
QList<QGraphicsItem*> pasteFromBinary(QByteArray const &itemData) {
    QDataStream inData(itemData);
    return readItems(inData);
}

QByteArray saveToByteArray(QList<QGraphicsItem *> items) {
    QByteArray itemData;
    QDataStream outData(&itemData, QIODevice::WriteOnly);
    saveItems(items, outData);
    return itemData;
}

void pasteToScene(QByteArray const &itemData, QGraphicsScene &s) {
    for (auto &&i: detail::pasteFromBinary(itemData))
        s.addItem(i);
    s.clearSelection();
    s.update();
}

// This is quite an ugly hack: We're pasting all elements (that were streamed
// into itemData before) into a temporary QGraphicsScene, which we can then
// just render as a whole. The former part is ugly, because we're relying on
// the data having been prepared to be copied to the clipboard already. The
// latter part is nice, though: There's no easy way to render just a few
// items, but QGraphicsScene::render() allows us to render all with a single
// call.
QImage renderToPixmap(QByteArray const &itemData) {
    QGraphicsScene s;
    pasteToScene(itemData, s);

    int const oversampling = 2;

    auto const boundingRect = s.itemsBoundingRect();
    int width = qCeil(boundingRect.width());
    int height = qCeil(boundingRect.height());
    QImage image(width * oversampling, height * oversampling, QImage::Format_RGB32);
    image.fill(Qt::white);
    image.setDotsPerMeterX(image.dotsPerMeterX()*oversampling);
    image.setDotsPerMeterY(image.dotsPerMeterY()*oversampling);
    QPainter painter(&image);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    s.render(&painter, image.rect(), boundingRect);
    return image;
}

void renderToSVG(const QString &filename, QByteArray const &itemData) {
    QGraphicsScene s;
    pasteToScene(itemData, s);

    QSize sceneSize = s.sceneRect().size().toSize();

    QSvgGenerator generator;
    generator.setFileName(filename);
    generator.setSize(sceneSize);
    generator.setViewBox(QRect(0, 0, sceneSize.width(), sceneSize.height()));
    generator.setDescription("Whiteboard SVG");
    generator.setTitle(filename);
    QPainter painter;
    painter.begin(&generator);
    s.render(&painter);
}


}

QList<QGraphicsItem*> pasteFromMimeData(QMimeData const *mimeData) {
    auto binData = mimeData->data(MIME_TYPE);
    return detail::pasteFromBinary(binData);
}

void copyGraphicsItems(QList<QGraphicsItem *> items) {
    if (items.isEmpty())
        return;

    auto const itemData = detail::saveToByteArray(items);
    auto const image = detail::renderToPixmap(itemData);

    QMimeData* mimeData = new QMimeData;
    mimeData->setData(MIME_TYPE, itemData);
    mimeData->setImageData(image);

    QApplication::clipboard()->setMimeData(mimeData);
}

void exportGraphicsItemsToFile(const QString &filename, QList<QGraphicsItem *> items) {
    auto const itemData = detail::saveToByteArray(items);

    if (filename.endsWith("png", Qt::CaseInsensitive)) {
        auto image = detail::renderToPixmap(itemData);
        image.save(filename);
    } else if (filename.endsWith("svg", Qt::CaseInsensitive)) {
        detail::renderToSVG(filename, itemData);
    } else {
        throw ExportFormatError();
    }
}
