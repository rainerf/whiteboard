#ifndef WHITEBOARDPIXMAPITEM_H
#define WHITEBOARDPIXMAPITEM_H

#include <QGraphicsPixmapItem>

#include "whiteboarditem.h"

class WhiteBoardPixmapItemBase : public QGraphicsPixmapItem {
public:
    enum { Type = UserType + 2 };
    int type() const override;

    WhiteBoardPixmapItemBase(QGraphicsItem *parent = nullptr);
    WhiteBoardPixmapItemBase(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

    void setOriginalPixmap(const QPixmap &pixmap);
    QPixmap originalPixmap() const;

    void setZoom(qreal zoom);
    qreal zoom() const;;
    qreal originalWidth() const;

private:
    QPixmap m_originalPixmap;
    qreal m_zoom;
};

using WhiteBoardPixmapItem = WhiteBoardItem<WhiteBoardPixmapItemBase>;

#endif // WHITEBOARDPIXMAPITEM_H
