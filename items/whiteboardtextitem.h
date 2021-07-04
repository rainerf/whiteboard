#ifndef WHITEBOARDTEXTITEM_H
#define WHITEBOARDTEXTITEM_H

#include <QGraphicsTextItem>

class WhiteBoardTextItem : public QGraphicsTextItem {
public:
    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    WhiteBoardTextItem(const QString &text, QGraphicsItem *parent = nullptr);
    WhiteBoardTextItem(QGraphicsItem *parent = nullptr);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // WHITEBOARDTEXTITEM_H
