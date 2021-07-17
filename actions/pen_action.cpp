#include <QColor>
#include <QPainter>
#include <QtGlobal>

#include "pen_action.h"

PenAction::PenAction(int thickness, QObject *parent) : QAction(parent), m_thickness(thickness) {
    connect(this, &QAction::triggered, [=]() { emit penSelected(m_thickness); });
    setCheckable(true);
    setIcon(createIcon());
}

QPixmap PenAction::createIcon() const {
    constexpr int GRID_SIZE = 30;
    QPixmap pixmap(QSize(GRID_SIZE, GRID_SIZE));
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen(QPen(QColor("black")));
    painter.drawEllipse(QPointF(GRID_SIZE / 2., GRID_SIZE / 2.), m_thickness / 2., m_thickness / 2.);
    painter.end();

    return pixmap;
}
