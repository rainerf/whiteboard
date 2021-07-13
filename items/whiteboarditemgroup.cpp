#include "whiteboarditemgroup.h"

int WhiteBoardItemGroupBase::type() const { return Type; }

WhiteBoardItemGroupBase::WhiteBoardItemGroupBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
}

void WhiteBoardItemGroupBase::setZoom(qreal zoom) {
    setScale(zoom);
}

qreal WhiteBoardItemGroupBase::originalWidth() const {
    // use the bounding rect of the base class instead of ours, since ours is overridden
    // by the one in the derived class that contains the handle...
    return QGraphicsItemGroup::boundingRect().width();
}
