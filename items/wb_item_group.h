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

#ifndef WHITEBOARDITEMGROUP_H
#define WHITEBOARDITEMGROUP_H

#include <QGraphicsItemGroup>

#include "wb_item.h"

class WB_ItemGroupBase : public QGraphicsItemGroup {
public:
    enum { Type = UserType + 3 };
    int type() const override;

    WB_ItemGroupBase(QGraphicsItem *parent = nullptr);

    void setZoom(qreal zoom);
    qreal originalWidth() const;
};

using WB_ItemGroup = WB_Item<WB_ItemGroupBase>;

#endif // WHITEBOARDITEMGROUP_H
