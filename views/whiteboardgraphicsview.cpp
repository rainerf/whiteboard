#include <QApplication>
#include <QClipboard>
#include <QDataStream>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include <QMimeData>
#include <QPixmap>
#include <QSettings>
#include <QTabletEvent>
#include <QtMath>

#include "whiteboardgraphicsview.h"
#include "items/whiteboardtextitem.h"

QDataStream &operator<<(QDataStream &out, QGraphicsItem *item) {
    out << item->pos() << item->scale() << item->rotation() << item->transform() << item->transformOriginPoint() << item->flags()
        << item->isEnabled() << item->zValue();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsItem *&item) {
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
QDataStream &operator>>(QDataStream &in, QAbstractGraphicsShapeItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsEllipseItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsPathItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsPolygonItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsRectItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsSimpleTextItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsLineItem *&item) {
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

QDataStream &operator>>(QDataStream &in, QGraphicsPixmapItem *&item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QPixmap pixmap;
    QPointF offset;
    int transformationMode;
    int shapeMode;
    in >> pixmap >> offset >> shapeMode >> transformationMode;
    item->setPixmap(pixmap);
    item->setOffset(offset);
    item->setTransformationMode(Qt::TransformationMode(transformationMode));
    item->setShapeMode(QGraphicsPixmapItem::ShapeMode(shapeMode));
    return in;
}

QDataStream &operator<<(QDataStream &out, QGraphicsTextItem *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->toHtml();
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsTextItem *&item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    QString html;
    in >> html;
    item->setHtml(html);
    return in;
}

void saveItem(QGraphicsItem *item, QDataStream &out, bool ignoreParent = false);
QGraphicsItem *readItem(QDataStream &in);

QDataStream &operator<<(QDataStream &out, QGraphicsItemGroup *item) {
    out << dynamic_cast<QGraphicsItem *>(item);
    auto children = item->childItems();
    out << children.size();
    for (auto &&i : item->childItems())
        saveItem(i, out, true);
    return out;
}

QDataStream &operator>>(QDataStream &in, QGraphicsItemGroup *&item) {
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;
    int size;
    in >> size;
    for (int i = 0; i < size; ++i) {
        if (auto x = readItem(in))
            item->addToGroup(x);
    }
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
    case QGraphicsPixmapItem::Type:
        out << dynamic_cast<QGraphicsPixmapItem *>(item);
        break;
    case QGraphicsTextItem::Type:
        out << dynamic_cast<QGraphicsTextItem *>(item);
        break;
    case QGraphicsItemGroup::Type:
        out << dynamic_cast<QGraphicsItemGroup *>(item);
        break;
    }
}

void saveItems(QList<QGraphicsItem *> items, QDataStream &out) {
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
    }
    return nullptr;
}

static QList<QGraphicsItem *> readItems(QDataStream &in) {
    QList<QGraphicsItem *> items;

    while (!in.atEnd()) {
        if (auto x = readItem(in))
            items << x;
    }
    return items;
}

void WhiteBoardGraphicsView::tabletEvent(QTabletEvent *event) {
    switch (event->type()) {
    case QEvent::TabletPress:
        setFocus();
        for (auto &&i : scene()->items())
            i->clearFocus();

        if (!m_deviceDown) {
            m_deviceDown = true;
            currentTool->handleTabletPress(*this, *event);
        }
        break;
    case QEvent::TabletMove:
        if (m_deviceDown) {
            currentTool->handleTabletMove(*this, *event);
        }
        break;
    case QEvent::TabletRelease:
        if (m_deviceDown && event->buttons() == Qt::NoButton) {
            m_deviceDown = false;
            currentTool->handleTabletRelease(*this, *event);
        }
        break;
    default:
        break;
    }
    event->accept();
}

void WhiteBoardGraphicsView::setColor(const QColor &color) {
    penTool.setColor(color);
    textTool.setColor(color);
    highlightTool.setColor(color);
}

void WhiteBoardGraphicsView::setPenThickness(int thickness) {
    penTool.setPen(thickness);
    highlightTool.setPen(thickness);
}

void WhiteBoardGraphicsView::setPenTool() {
    currentTool = &penTool;
}

void WhiteBoardGraphicsView::setTextTool() {
    currentTool = &textTool;
}

void WhiteBoardGraphicsView::setHighlightTool() {
    currentTool = &highlightTool;
}

void WhiteBoardGraphicsView::setPointerTool() {
    currentTool = &pointerTool;
}

void WhiteBoardGraphicsView::deleteSelectedItems() {
    for (auto &&i : scene()->selectedItems()) {
        scene()->removeItem(i);
    }
}

void WhiteBoardGraphicsView::setFont(const QFont &font) {
    textTool.setFont(font);
}

void WhiteBoardGraphicsView::setFontSize(int size) {
    textTool.setFontSize(size);
}

void WhiteBoardGraphicsView::paste() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasImage()) {
        auto item = new QGraphicsPixmapItem(qvariant_cast<QPixmap>(mimeData->imageData()));
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        scene()->addItem(item);
    } else if (mimeData->hasHtml()) {
        auto myTextItem = new WhiteBoardTextItem();
        myTextItem->setHtml(mimeData->html());
        scene()->addItem(myTextItem);
    } else if (mimeData->hasText()) {
        auto myTextItem = new WhiteBoardTextItem();
        myTextItem->setPlainText(mimeData->text());
        scene()->addItem(myTextItem);
    }
}

QImage WhiteBoardGraphicsView::renderToPixmap() {
    auto const boundingRect = scene()->itemsBoundingRect();
    int width = qCeil(boundingRect.width());
    int height = qCeil(boundingRect.height());
    QImage pixmap(width * 8, height * 8, QImage::Format_ARGB32);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    scene()->render(&painter, pixmap.rect(), boundingRect);
    return pixmap;
}

void WhiteBoardGraphicsView::copy() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(renderToPixmap());
}

void WhiteBoardGraphicsView::selectAll() {
    for (auto &&i : scene()->items())
        i->setSelected(true);
}

void WhiteBoardGraphicsView::saveToFile() {
    QFile fileOut("/home/rainer/file.txt");
    if (fileOut.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileOut);
        saveItems(scene()->items(), out);
        fileOut.close();
    }
}

void WhiteBoardGraphicsView::loadFromFile() {
    QFile fileIn("/home/rainer/file.txt");
    if (fileIn.open(QIODevice::ReadOnly)) {
        QDataStream in(&fileIn);
        QList<QGraphicsItem *> items = readItems(in);
        for (QGraphicsItem *item : items) {
            scene()->addItem(item);
        }
    }
}

void WhiteBoardGraphicsView::clear() {
    scene()->clear();
}
