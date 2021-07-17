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
    qreal zoom() const;;
    qreal originalWidth() const;

private:
    QPixmap m_originalPixmap;
    qreal m_zoom = 1;
};

using WB_PixmapItem = WB_Item<WB_PixmapItemBase>;

#endif // WHITEBOARDPIXMAPITEM_H
