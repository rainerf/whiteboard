#include "wb_pixmap_item.h"

int WB_PixmapItemBase::type() const { return Type; }

WB_PixmapItemBase::WB_PixmapItemBase(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    setTransformationMode(Qt::SmoothTransformation);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

WB_PixmapItemBase::WB_PixmapItemBase(const QPixmap &pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap, parent), m_originalPixmap(pixmap) {
    setTransformationMode(Qt::SmoothTransformation);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

void WB_PixmapItemBase::setOriginalPixmap(const QPixmap &pixmap) {
    m_originalPixmap = pixmap;
    QGraphicsPixmapItem::setPixmap(pixmap);
}

QPixmap WB_PixmapItemBase::originalPixmap() const {
    return m_originalPixmap;
}

void WB_PixmapItemBase::setZoom(qreal zoom) {
    setPixmap(m_originalPixmap.scaled(m_originalPixmap.size()*zoom, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_zoom = zoom;
}

qreal WB_PixmapItemBase::zoom() const { return m_zoom; }

qreal WB_PixmapItemBase::originalWidth() const {
    return m_originalPixmap.width();
}
