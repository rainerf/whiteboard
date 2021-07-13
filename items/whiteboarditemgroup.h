#ifndef WHITEBOARDITEMGROUP_H
#define WHITEBOARDITEMGROUP_H

#include <QGraphicsItemGroup>

#include "whiteboarditem.h"

class WhiteBoardItemGroupBase : public QGraphicsItemGroup {
public:
    enum { Type = UserType + 3 };
    int type() const override;

    WhiteBoardItemGroupBase(QGraphicsItem *parent = nullptr);

    void setZoom(qreal zoom);
    qreal originalWidth() const;
};

using WhiteBoardItemGroup = WhiteBoardItem<WhiteBoardItemGroupBase>;

#endif // WHITEBOARDITEMGROUP_H
