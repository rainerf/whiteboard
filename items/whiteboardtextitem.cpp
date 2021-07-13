#include <QTextCursor>
#include <QKeyEvent>

#include "whiteboardtextitem.h"

int WhiteBoardTextItemBase::type() const { return Type; }

WhiteBoardTextItemBase::WhiteBoardTextItemBase(const QString &text, QGraphicsItem *parent) : QGraphicsTextItem(text, parent) {
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::NoTextInteraction);
}

WhiteBoardTextItemBase::WhiteBoardTextItemBase(QGraphicsItem *parent) : WhiteBoardTextItemBase("", parent) {
}

void WhiteBoardTextItemBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) {
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();
}

void WhiteBoardTextItemBase::focusOutEvent(QFocusEvent *) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    setTextCursor(QTextCursor(document()));
}

void WhiteBoardTextItemBase::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        clearFocus();
    }
    QGraphicsTextItem::keyPressEvent(event);
}

void WhiteBoardTextItemBase::setZoom(qreal zoom) {
    setScale(zoom);
}

qreal WhiteBoardTextItemBase::originalWidth() const {
    // use the bounding rect of the base class instead of ours, since ours is overridden
    // by the one in the derived class that contains the handle...
    return QGraphicsTextItem::boundingRect().width();
}
