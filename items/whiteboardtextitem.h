#ifndef WHITEBOARDTEXTITEM_H
#define WHITEBOARDTEXTITEM_H

#include <QGraphicsTextItem>

class WhiteBoardTextItem : public QGraphicsTextItem {
public:
    WhiteBoardTextItem(const QString &text, QGraphicsItem *parent = nullptr);
    WhiteBoardTextItem(QGraphicsItem *parent = nullptr);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
    void focusOutEvent(QFocusEvent *event) override;
};

#endif // WHITEBOARDTEXTITEM_H
