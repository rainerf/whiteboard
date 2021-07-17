#include "wb_item_group.h"

int WB_ItemGroupBase::type() const { return Type; }

WB_ItemGroupBase::WB_ItemGroupBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
}

void WB_ItemGroupBase::setZoom(qreal zoom) {
    setScale(zoom);
}

qreal WB_ItemGroupBase::originalWidth() const {
    // use the bounding rect of the base class instead of ours, since ours is overridden
    // by the one in the derived class that contains the handle...
    return QGraphicsItemGroup::boundingRect().width();
}
