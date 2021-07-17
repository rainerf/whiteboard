#ifndef WHITEBOARDTEXTITEM_H
#define WHITEBOARDTEXTITEM_H

#include <QGraphicsTextItem>

#include "wb_item.h"

class WB_TextItemBase : public QGraphicsTextItem {
public:
    enum { Type = UserType + 1 };
    int type() const override;

    WB_TextItemBase(const QString &text, QGraphicsItem *parent = nullptr);
    WB_TextItemBase(QGraphicsItem *parent = nullptr);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setZoom(qreal zoom);
    qreal originalWidth() const;
};

using WB_TextItem = WB_Item<WB_TextItemBase>;

#endif // WHITEBOARDTEXTITEM_H
