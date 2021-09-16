#include "wb_svg_item.h"

#include <QSvgRenderer>

void WB_SvgItemBase::setZoom(qreal zoom) {
    setScale(zoom);
}

qreal WB_SvgItemBase::originalWidth() const {
    return renderer()->viewBoxF().width();
}
