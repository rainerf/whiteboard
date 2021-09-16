#include "wb_svg_item.h"

#include <QFile>

int WB_SvgItemBase::type() const {
    return Type;
}

WB_SvgItemBase::WB_SvgItemBase(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        throw SvgFileOpenError();

    setBinData(file.readAll());
}

void WB_SvgItemBase::setZoom(qreal zoom) {
    setScale(zoom);
}

qreal WB_SvgItemBase::originalWidth() const {
    return renderer()->viewBoxF().width();
}

const QByteArray &WB_SvgItemBase::binData() const {
    return m_binData;
}

void WB_SvgItemBase::setBinData(QByteArray &&data) {
    m_binData = std::move(data);
    m_renderer.load(m_binData);
    setSharedRenderer(&m_renderer);
}
