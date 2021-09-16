#ifndef WB_SVGITEM_H
#define WB_SVGITEM_H

#include "wb_item.h"

#include <QGraphicsSvgItem>
#include <QSvgRenderer>

class SvgFileOpenError : public std::exception {};

class WB_SvgItemBase : public QGraphicsSvgItem {
public:
    enum { Type = UserType + 4 };
    int type() const override;

    WB_SvgItemBase(QString const &filename);
    WB_SvgItemBase() = default;

    void setZoom(qreal zoom);
    qreal originalWidth() const;

    QByteArray const &binData() const;
    void setBinData(QByteArray &&data);

private:
    QSvgRenderer m_renderer;
    QByteArray m_binData;
};

using WB_SvgItem = WB_Item<WB_SvgItemBase>;

#endif // WB_SVGITEM_H
