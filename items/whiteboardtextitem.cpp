#include <QTextCursor>
#include <QKeyEvent>

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

void WhiteBoardTextItem::focusOutEvent(QFocusEvent *) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    setTextCursor(QTextCursor(document()));
}

void WhiteBoardTextItem::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        clearFocus();
    }
    QGraphicsTextItem::keyPressEvent(event);
}
