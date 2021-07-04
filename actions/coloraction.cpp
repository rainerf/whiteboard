#include "coloraction.h"

#include <QPainter>

ColorAction::ColorAction(const QColor &color, QObject *parent) : QAction(parent), m_color(color) {
    connect(this, &QAction::triggered, [=]() { emit colorSelected(m_color); });
    setCheckable(true);
    setIcon(createIcon());
}

QPixmap ColorAction::createIcon() {
    constexpr int GRID_SIZE = 20;
    QPixmap pixmap(QSize(GRID_SIZE, GRID_SIZE));
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(m_color));
    painter.setPen(QPen(m_color));
    painter.drawEllipse(1, 1, GRID_SIZE - 2, GRID_SIZE - 2);
    painter.end();

    return pixmap;
}
