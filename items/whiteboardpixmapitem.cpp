#include "whiteboardpixmapitem.h"

int WhiteBoardPixmapItemBase::type() const { return Type; }

WhiteBoardPixmapItemBase::WhiteBoardPixmapItemBase(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    setTransformationMode(Qt::SmoothTransformation);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

WhiteBoardPixmapItemBase::WhiteBoardPixmapItemBase(const QPixmap &pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap, parent), m_originalPixmap(pixmap) {
    setTransformationMode(Qt::SmoothTransformation);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

void WhiteBoardPixmapItemBase::setOriginalPixmap(const QPixmap &pixmap) {
    m_originalPixmap = pixmap;
    QGraphicsPixmapItem::setPixmap(pixmap);
}

QPixmap WhiteBoardPixmapItemBase::originalPixmap() const {
    return m_originalPixmap;
}

void WhiteBoardPixmapItemBase::setZoom(qreal zoom) {
    setPixmap(m_originalPixmap.scaled(m_originalPixmap.size()*zoom, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_zoom = zoom;
}

qreal WhiteBoardPixmapItemBase::zoom() const { return m_zoom; }

qreal WhiteBoardPixmapItemBase::originalWidth() const {
    return m_originalPixmap.width();
}
