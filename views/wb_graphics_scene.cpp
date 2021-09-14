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

#include "wb_graphics_view.h"
#include "wb_graphics_scene.h"

#include <QGraphicsScene>
#include <QMessageBox>
#include <QFile>
#include <QPainter>

#include "lib/qgraphicsscene_storage.h"
#include "lib/qgraphicsscene_dump.h"

namespace detail {

QPixmap createTexture() {
    constexpr int GRID_SIZE = 40;
    QPixmap pixmap(QSize(GRID_SIZE, GRID_SIZE));
    pixmap.fill();

    QPainter painter(&pixmap);
    QPen penHLines(QColor(200, 200, 200), 1, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
    painter.setPen(penHLines);
    painter.drawLine(0, 0, 0, GRID_SIZE);
    painter.drawLine(0, 0, GRID_SIZE, 0);
    painter.end();

    return pixmap;
}

} // namespace detail

WB_GraphicsScene::WB_GraphicsScene(WB_GraphicsView *parent) : QObject(parent), m_scene(new QGraphicsScene(this)) {
    parent->setScene(m_scene);
}

void WB_GraphicsScene::saveToFile(const QString &filename) {
    QFile fileOut(filename);
    if (fileOut.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileOut);
        saveItems(m_scene->items(), out);
        fileOut.close();
    }
}

void WB_GraphicsScene::loadFromFile(const QString &filename) {
    QFile fileIn(filename);
    if (fileIn.open(QIODevice::ReadOnly)) {
        m_scene->clear();

        QDataStream in(&fileIn);
        QList<QGraphicsItem *> items = readItems(in);
        for (QGraphicsItem *item : items) {
            m_scene->addItem(item);
        }
    }

    calculateNextZs();
}

void WB_GraphicsScene::debugDumpAllItems() {
    QTextStream out(stdout, QIODevice::WriteOnly);
    dumpItems(m_scene->items(), out);
}

QList<QGraphicsItem *> WB_GraphicsScene::items(const QRectF &rect, Qt::ItemSelectionMode mode, Qt::SortOrder order, const QTransform &deviceTransform) const {
    return m_scene->items(rect, mode, order, deviceTransform);
}

QGraphicsItem *WB_GraphicsScene::itemAt(const QPointF &position, const QTransform &deviceTransform) const {
    return m_scene->itemAt(position, deviceTransform);
}

QList<QGraphicsItem*> WB_GraphicsScene::topLevelItems() {
    QList<QGraphicsItem*> ret;
    for (auto &&i: m_scene->items())
        if (!i->parentItem())
            ret.append(i);
    return ret;
}

QList<QGraphicsItem *> WB_GraphicsScene::selectedItems() {
    return m_scene->selectedItems();
}

void WB_GraphicsScene::addItem(QGraphicsItem *item) {
    m_scene->addItem(item);
}

void WB_GraphicsScene::removeItem(QGraphicsItem *item) {
    m_scene->removeItem(item);
}

void WB_GraphicsScene::update() {
    m_scene->update();
}

void WB_GraphicsScene::clearSelection() {
    m_scene->clearSelection();
}

void WB_GraphicsScene::setGrid(bool enabled) {
    if (enabled)
        m_scene->setBackgroundBrush(QBrush(detail::createTexture()));
    else
        m_scene->setBackgroundBrush(QBrush());
}

qreal WB_GraphicsScene::getNewBackgroundZ() {
    return m_nextBackgroundZ++;
}

qreal WB_GraphicsScene::getNewForegroundZ() {
    return m_nextForegroundZ++;
}

qreal WB_GraphicsScene::getAbsoluteForegroundZ() {
    return std::numeric_limits<qreal>::max();
}

void WB_GraphicsScene::calculateNextZs() {
    qreal currentBackgroundZ = INITIAL_BACKGROUND_Z;
    qreal currentForegroundZ = INITIAL_FOREGROUND_Z;

    static_assert(INITIAL_BACKGROUND_Z < 0, "INITIAL_BACKGROUND_Z has to be negative");
    static_assert(INITIAL_FOREGROUND_Z >= 0, "INITIAL_FOREGROUND_Z has to be 0 or positive");

    for (auto &&i: topLevelItems()) {
        if (i->zValue() < 0)
            currentBackgroundZ = std::max(i->zValue(), currentBackgroundZ);
        else
            currentForegroundZ = std::max(i->zValue(), currentForegroundZ);
    }

    m_nextBackgroundZ = currentBackgroundZ + 1;
    m_nextForegroundZ = currentForegroundZ + 1;
}
