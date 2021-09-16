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

#ifndef WHITEBOARDPIXMAPITEM_H
#define WHITEBOARDPIXMAPITEM_H

#include <QGraphicsPixmapItem>

#include "wb_item.h"

class WB_PixmapItemBase : public QGraphicsPixmapItem {
public:
    enum { Type = UserType + 2 };
    int type() const override;

    WB_PixmapItemBase(QGraphicsItem *parent = nullptr);
    WB_PixmapItemBase(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

    void setOriginalPixmap(const QPixmap &pixmap);
    QPixmap originalPixmap() const;

    void setZoom(qreal zoom);
    qreal zoom() const;
    qreal originalWidth() const;

private:
    QPixmap m_originalPixmap;
    qreal m_zoom = 1;
};

using WB_PixmapItem = WB_Item<WB_PixmapItemBase>;

#endif // WHITEBOARDPIXMAPITEM_H
