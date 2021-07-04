#include <QTextCursor>

#include "whiteboardtextitem.h"

WhiteBoardTextItem::WhiteBoardTextItem(const QString &text, QGraphicsItem *parent) : QGraphicsTextItem(text, parent) {
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::NoTextInteraction);
}

WhiteBoardTextItem::WhiteBoardTextItem(QGraphicsItem *parent) : WhiteBoardTextItem("", parent) {
}

void WhiteBoardTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) {
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();
}
