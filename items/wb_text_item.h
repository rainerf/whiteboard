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

#ifndef WHITEBOARDTEXTITEM_H
#define WHITEBOARDTEXTITEM_H

#include <QGraphicsTextItem>

#include "wb_item.h"

class WB_TextItemBase : public QGraphicsTextItem {
public:
    enum { Type = UserType + 1 };
    int type() const override;

    WB_TextItemBase(const QString &text, QGraphicsItem *parent = nullptr);
    WB_TextItemBase(QGraphicsItem *parent = nullptr);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setZoom(qreal zoom);
    qreal originalWidth() const;
};

using WB_TextItem = WB_Item<WB_TextItemBase>;

#endif // WHITEBOARDTEXTITEM_H
