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
