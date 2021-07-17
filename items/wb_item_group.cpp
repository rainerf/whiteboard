// This file is part of Whiteboard.
//
// Whiteboard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Whiteboard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Whiteboard.  If not, see <https://www.gnu.org/licenses/>.

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
