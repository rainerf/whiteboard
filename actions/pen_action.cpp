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
