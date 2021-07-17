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
