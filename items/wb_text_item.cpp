#include <QTextCursor>
#include <QKeyEvent>

#include "wb_text_item.h"

int WB_TextItemBase::type() const { return Type; }

WB_TextItemBase::WB_TextItemBase(const QString &text, QGraphicsItem *parent) : QGraphicsTextItem(text, parent) {
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::NoTextInteraction);
}

WB_TextItemBase::WB_TextItemBase(QGraphicsItem *parent) : WB_TextItemBase("", parent) {
}

void WB_TextItemBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) {
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();
}

void WB_TextItemBase::focusOutEvent(QFocusEvent *) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    setTextCursor(QTextCursor(document()));
}

void WB_TextItemBase::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        clearFocus();
    }
    QGraphicsTextItem::keyPressEvent(event);
}

void WB_TextItemBase::setZoom(qreal zoom) {
    setScale(zoom);
}

qreal WB_TextItemBase::originalWidth() const {
    // use the bounding rect of the base class instead of ours, since ours is overridden
    // by the one in the derived class that contains the handle...
    return QGraphicsTextItem::boundingRect().width();
}
