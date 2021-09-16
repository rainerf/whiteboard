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

#include "lib/qgraphicsscene_storage.h"

#include <QPen>
#include <QBrush>
#include <QFont>
#include "items/wb_text_item.h"
#include "items/wb_item_group.h"
#include "items/wb_pixmap_item.h"
#include "items/wb_svg_item.h"

// inspired by https://stackoverflow.com/a/51498180/1683161

static constexpr quint32 magicNumber = 0x84c2adc1;
static constexpr quint32 version = 1;

QDataStream &operator<<(QDataStream &out, QGraphicsItem *item) {
    out << item->pos() << item->scale() << item->rotation() << item->transform() << item->transformOriginPoint() << item->flags()
        << item->isEnabled() << item->zValue();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsItem *item) {
    QPointF pos;
    qreal scale;
    qreal rotation;
    QTransform transform;
    QPointF transformOriginPoint;
    QGraphicsItem::GraphicsItemFlags flags;
    bool isEnabled;
    qreal zValue;
    in >> pos >> scale >> rotation >> transform >> transformOriginPoint >> flags >> isEnabled >> zValue;
    item->setPos(pos);
    item->setScale(scale);
    item->setRotation(rotation);
    item->setTransform(transform);
    item->setTransformOriginPoint(transformOriginPoint);
    item->setFlags(flags);
    item->setEnabled(isEnabled);
    item->setZValue(zValue);

    return in;
}

QDataStream &operator<<(QDataStream &out, QAbstractGraphicsShapeItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() << item->brush();
    return out;
}

QDataStream &operator>>(QDataStream &in, QAbstractGraphicsShapeItem *item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QPen pen;
    QBrush brush;
    in >> pen >> brush;
    item->setBrush(brush);
    item->setPen(pen);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsEllipseItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->rect() << item->startAngle() << item->spanAngle();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsEllipseItem *item) {
    QAbstractGraphicsShapeItem *tmp = dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    in >> tmp;
    QRectF rect;
    int startAngle;
    int spanAngle;
    in >> rect >> startAngle >> spanAngle;
    item->setRect(rect);
    item->setStartAngle(startAngle);
    item->setSpanAngle(spanAngle);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsPathItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->path();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsPathItem *item) {
    QAbstractGraphicsShapeItem *tmp = dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    in >> tmp;
    QPainterPath path;
    in >> path;
    item->setPath(path);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsPolygonItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->polygon() << item->fillRule();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsPolygonItem *item) {
    QAbstractGraphicsShapeItem *tmp = dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    in >> tmp;
    QPolygonF polygon;
    int rule;
    in >> polygon >> rule;
    item->setPolygon(polygon);
    item->setFillRule(Qt::FillRule(rule));
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsRectItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->rect();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsRectItem *item) {
    QAbstractGraphicsShapeItem *tmp = dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    in >> tmp;
    QRectF rect;
    in >> rect;
    item->setRect(rect);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsSimpleTextItem *item) {
    out << dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    out << item->text() << item->font();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsSimpleTextItem *item) {
    QAbstractGraphicsShapeItem *tmp = dynamic_cast<QAbstractGraphicsShapeItem *>(item);
    in >> tmp;
    QString text;
    QFont font;
    in >> text >> font;
    item->setText(text);
    item->setFont(font);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsLineItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() << item->line();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsLineItem *item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QPen pen;
    QLineF line;
    in >> pen >> line;
    item->setPen(pen);
    item->setLine(line);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsPixmapItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pixmap() << item->offset() << item->transformationMode() << item->shapeMode();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsPixmapItem *item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QPixmap pixmap;
    QPointF offset;
    int transformationMode;
    int shapeMode;
    in >> pixmap >> offset >> transformationMode >> shapeMode;
    item->setPixmap(pixmap);
    item->setOffset(offset);
    item->setTransformationMode(Qt::TransformationMode(transformationMode));
    item->setShapeMode(QGraphicsPixmapItem::ShapeMode(shapeMode));
    return in;
}

QDataStream &operator<<(QDataStream &out, WB_PixmapItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->originalPixmap() << item->offset() << item->transformationMode() << item->shapeMode() << item->zoom();
    return out;
}

QDataStream &operator>>(QDataStream &in, WB_PixmapItem *item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QPixmap pixmap;
    QPointF offset;
    int transformationMode;
    int shapeMode;
    qreal zoom;
    in >> pixmap >> offset >> transformationMode >> shapeMode >> zoom;
    item->setOriginalPixmap(pixmap);
    item->setOffset(offset);
    item->setTransformationMode(Qt::TransformationMode(transformationMode));
    item->setShapeMode(QGraphicsPixmapItem::ShapeMode(shapeMode));
    item->setZoom(zoom);
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsTextItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->toHtml();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsTextItem *item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QString html;
    in >> html;
    item->setHtml(html);
    return in;
}

QDataStream &operator<<(QDataStream &out, WB_SvgItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->binData();
    return out;
}

QDataStream &operator>>(QDataStream &in, WB_SvgItem *item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QByteArray data;
    in >> data;
    item->setBinData(std::move(data));
    return in;
}

void saveItem(QGraphicsItem *item, QDataStream &out, bool ignoreParent = false);
QGraphicsItem *readItem(QDataStream &in);

QDataStream &operator<<(QDataStream &out, QGraphicsItemGroup *item) {
    auto children = item->childItems();
    out << children.size();
    for (auto &&i : item->childItems())
        saveItem(i, out, true);
    out << dynamic_cast<QGraphicsItem *>(item);
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsItemGroup *item) {
    int size;
    in >> size;
    for (int i = 0; i < size; ++i) {
        if (auto x = readItem(in))
            item->addToGroup(x);
    }

    // NB: we need to restore the position of the Group *after* adding the items,
    // otherwise we're going to mess up the original positioning
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;

    return in;
}

void saveItem(QGraphicsItem *item, QDataStream &out, bool ignoreParent) {
    if (item->parentItem() && !ignoreParent)
        return;

    out << item->type();
    switch (item->type()) {
    case QGraphicsLineItem::Type:
        out << dynamic_cast<QGraphicsLineItem *>(item);
        break;
    case QGraphicsSimpleTextItem::Type:
        out << dynamic_cast<QGraphicsSimpleTextItem *>(item);
        break;
    case QGraphicsRectItem::Type:
        out << dynamic_cast<QGraphicsRectItem *>(item);
        break;
    case QGraphicsPolygonItem::Type:
        out << dynamic_cast<QGraphicsPolygonItem *>(item);
        break;
    case QGraphicsPathItem::Type:
        out << dynamic_cast<QGraphicsPathItem *>(item);
        break;
    case WB_PixmapItem::Type:
        out << dynamic_cast<WB_PixmapItem *>(item);
        break;
    case QGraphicsPixmapItem::Type:
        out << dynamic_cast<QGraphicsPixmapItem *>(item);
        break;
    case WB_TextItem::Type:
        // type was already serialized above, otherwise, it's the same thing as
        // the general type (from a data point of view), so there's nothing
        // special to do here
    case QGraphicsTextItem::Type:
        out << dynamic_cast<QGraphicsTextItem *>(item);
        break;
    case WB_ItemGroup::Type:
        // type was already serialized above, otherwise, it's the same thing as
        // the general type (from a data point of view), so there's nothing
        // special to do here
    case QGraphicsItemGroup::Type:
        out << dynamic_cast<QGraphicsItemGroup *>(item);
        break;
    case WB_SvgItem::Type:
        out << dynamic_cast<WB_SvgItem *>(item);
        break;
    }
}

void saveItems(QList<QGraphicsItem *> items, QDataStream &out) {
    out << magicNumber;
    out << version;

    for (QGraphicsItem *item : items) {
        saveItem(item, out);
    }
}

QGraphicsItem *readItem(QDataStream &in) {
    int type;
    in >> type;
    switch (type) {
    case QGraphicsLineItem::Type: {
        QGraphicsLineItem *item = new QGraphicsLineItem;
        in >> item;
        return item;
    }
    case QGraphicsSimpleTextItem::Type: {
        QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem;
        in >> item;
        return item;
        break;
    }
    case QGraphicsRectItem::Type: {
        QGraphicsRectItem *item = new QGraphicsRectItem;
        in >> item;
        return item;
        break;
    }
    case QGraphicsPolygonItem::Type: {
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem;
        in >> item;
        return item;
        break;
    }
    case QGraphicsPathItem::Type: {
        QGraphicsPathItem *item = new QGraphicsPathItem;
        in >> item;
        return item;
        break;
    }
    case QGraphicsPixmapItem::Type: {
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem;
        in >> item;
        return item;
        break;
    }
    case QGraphicsTextItem::Type: {
        QGraphicsTextItem *item = new QGraphicsTextItem;
        in >> item;
        return item;
        break;
    }
    case QGraphicsItemGroup::Type: {
        QGraphicsItemGroup *item = new QGraphicsItemGroup;
        in >> item;
        return item;
        break;
    }
    case WB_TextItem::Type: {
        WB_TextItem *item = new WB_TextItem;
        in >> static_cast<QGraphicsTextItem*>(item);
        return item;
        break;
    }
    case WB_ItemGroup::Type: {
        WB_ItemGroup *item = new WB_ItemGroup;
        in >> static_cast<QGraphicsItemGroup*>(item);
        return item;
        break;
    }
    case WB_PixmapItem::Type: {
        WB_PixmapItem *item = new WB_PixmapItem;
        in >> item;
        return item;
        break;
    }
    case WB_SvgItem::Type: {
        WB_SvgItem *item = new WB_SvgItem;
        in >> item;
        return item;
        break;
    }
    }
    return nullptr;
}

QList<QGraphicsItem *> readItems(QDataStream &in) {
    QList<QGraphicsItem *> items;

    quint32 magicNumber_, version_;
    in >> magicNumber_ >> version_;
    if (magicNumber_ != magicNumber) {
        throw FileFormatError();
    }
    if (version_ != version) {
        throw FileVersionError();
    }

    while (!in.atEnd()) {
        if (auto x = readItem(in))
            items << x;
    }
    return items;
}
