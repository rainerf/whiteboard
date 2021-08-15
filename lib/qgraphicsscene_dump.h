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

#ifndef QGRAPHICSSCENE_DUMP_H
#define QGRAPHICSSCENE_DUMP_H

#include <QTextStream>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPointF>
#include "items/wb_text_item.h"
#include "items/wb_item_group.h"
#include "items/wb_pixmap_item.h"

// inspired by https://stackoverflow.com/a/51498180/1683161

QTextStream &operator<<(QTextStream &out, QPointF item) {
    out << "QPointF(" << item.x() << "," << item.y() << ")";
    return out;
}

QTextStream &operator<<(QTextStream &out, QRectF item) {
    out << "QRectF(" << item.x() << "," << item.y() << "," << item.width() << "," << item.height() << ")\n";
    return out;
}

QTextStream &operator<<(QTextStream &out, QLineF item) {
    out << "QLineF" << item.p1() << "," << item.p2() << ")\n";
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsItem *item) {
    out << "QGraphicsItem(" << item->pos() << "," << item->scale() << "," << item->rotation() << "," << /*item->transform() << "," <<*/ item->transformOriginPoint() << "," << item->flags() << ","
        << item->isEnabled() << "," << item->zValue() << ")\n";
    return out;
}

QTextStream &operator<<(QTextStream &out, QColor color) {
    out << "QColor(" << color.red() << "," << color.green() << "," << color.blue() << ")";
    return out;
}

QTextStream &operator<<(QTextStream &out, QPen pen) {
    out << "QPen(" << pen.color() << ")\n";
    return out;
}

QTextStream &operator<<(QTextStream &out, QBrush brush) {
    out << "QBrush(" << brush.color() << ")\n";
    return out;
}

QTextStream &operator<<(QTextStream &out, QAbstractGraphicsShapeItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() << item->brush();
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsEllipseItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->rect() << item->startAngle() << item->spanAngle();
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsPathItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    /*out << item->path()*/;
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsPolygonItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << /*item->polygon() << */item->fillRule();
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsRectItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->rect();
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsSimpleTextItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->text() /*<< item->font()*/;
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsLineItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() << item->line();
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsPixmapItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << /*item->pixmap() << */item->offset() << item->transformationMode() << item->shapeMode();
    return out;
}

QTextStream &operator<<(QTextStream &out, WB_PixmapItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << /*item->originalPixmap() << */item->offset() << item->transformationMode() << item->shapeMode() << item->zoom();
    return out;
}

QTextStream &operator<<(QTextStream &out, QGraphicsTextItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->toHtml();
    return out;
}

void dumpItem(QGraphicsItem *item, QTextStream &out, bool ignoreParent = false);

QTextStream &operator<<(QTextStream &out, QGraphicsItemGroup *item) {
    auto children = item->childItems();
    out << "children: " << children.size() << "\n";
    for (auto &&i : item->childItems())
        dumpItem(i, out, true);
    out << dynamic_cast<QGraphicsItem *>(item);
    return out;
}

void dumpItem(QGraphicsItem *item, QTextStream &out, bool ignoreParent) {
    if (item->parentItem() && !ignoreParent)
        return;

    switch (item->type()) {
    case QGraphicsLineItem::Type:
        out << "QGraphicsLineItem {\n";
        out << dynamic_cast<QGraphicsLineItem *>(item);
        out << "} // QGraphicsLineItem\n";
        break;
    case QGraphicsSimpleTextItem::Type:
        out << "QGraphicsSimpleTextItem {\n";
        out << dynamic_cast<QGraphicsSimpleTextItem *>(item);
        out << "} // QGraphicsSimpleTextItem\n";
        break;
    case QGraphicsRectItem::Type:
        out << "QGraphicsRectItem {\n";
        out << dynamic_cast<QGraphicsRectItem *>(item);
        out << "} // QGraphicsRectItem\n";
        break;
    case QGraphicsPolygonItem::Type:
        out << "QGraphicsPolygonItem {\n";
        out << dynamic_cast<QGraphicsPolygonItem *>(item);
        out << "} // QGraphicsPolygonItem\n";
        break;
    case QGraphicsPathItem::Type:
        out << "QGraphicsPathItem {\n";
        out << dynamic_cast<QGraphicsPathItem *>(item);
        out << "} // QGraphicsPathItem\n";
        break;
    case WB_PixmapItem::Type:
        out << "WB_PixmapItem {\n";
        out << dynamic_cast<WB_PixmapItem *>(item);
        out << "} // WB_PixmapItem\n";
        break;
    case QGraphicsPixmapItem::Type:
        out << "QGraphicsPixmapItem {\n";
        out << dynamic_cast<QGraphicsPixmapItem *>(item);
        out << "} // QGraphicsPixmapItem\n";
        break;
    case WB_TextItem::Type:
        // type was already serialized above, otherwise, it's the same thing as
        // the general type (from a data point of view), so there's nothing
        // special to do here
    case QGraphicsTextItem::Type:
        out << "QGraphicsTextItem {\n";
        out << dynamic_cast<QGraphicsTextItem *>(item);
        out << "} // QGraphicsTextItem\n";
        break;
    case WB_ItemGroup::Type:
        // type was already serialized above, otherwise, it's the same thing as
        // the general type (from a data point of view), so there's nothing
        // special to do here
    case QGraphicsItemGroup::Type:
        out << "QGraphicsItemGroup {\n";
        out << dynamic_cast<QGraphicsItemGroup *>(item);
        out << "} // QGraphicsItemGroup\n";
        break;
    }
}

void dumpItems(QList<QGraphicsItem *> items, QTextStream &out) {
    for (QGraphicsItem *item : items) {
        dumpItem(item, out);
    }
}

#endif // QGRAPHICSSCENE_DUMP_H
