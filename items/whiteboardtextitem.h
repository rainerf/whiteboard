#ifndef WHITEBOARDTEXTITEM_H
#define WHITEBOARDTEXTITEM_H

#include <QGraphicsTextItem>

#include "whiteboarditem.h"

class WhiteBoardTextItemBase : public QGraphicsTextItem {
public:
    enum { Type = UserType + 1 };
    int type() const override;

    WhiteBoardTextItemBase(const QString &text, QGraphicsItem *parent = nullptr);
    WhiteBoardTextItemBase(QGraphicsItem *parent = nullptr);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setZoom(qreal zoom);
    qreal originalWidth() const;
};

using WhiteBoardTextItem = WhiteBoardItem<WhiteBoardTextItemBase>;

#endif // WHITEBOARDTEXTITEM_H
