#ifndef WB_SVGITEM_H
#define WB_SVGITEM_H

#include "wb_item.h"

#include <QGraphicsSvgItem>

class WB_SvgItemBase : public QGraphicsSvgItem {
public:
    using QGraphicsSvgItem::QGraphicsSvgItem;

    void setZoom(qreal zoom);
    qreal originalWidth() const;
};

using WB_SvgItem = WB_Item<WB_SvgItemBase>;

#endif // WB_SVGITEM_H
