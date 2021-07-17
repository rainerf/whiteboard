#include "wb_graphics_scene.h"

#include <QMessageBox>
#include <QFile>
#include <QPainter>

#include "lib/qgraphicsscene_storage.h"

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

WB_GraphicsScene::WB_GraphicsScene(QWidget *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(QBrush(detail::createTexture()));
}

void WB_GraphicsScene::saveToFile(const QString &filename) {
    QFile fileOut(filename);
    if (fileOut.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileOut);
        saveItems(items(), out);
        fileOut.close();
    }
}

void WB_GraphicsScene::loadFromFile(const QString &filename) {
    QFile fileIn(filename);
    if (fileIn.open(QIODevice::ReadOnly)) {
        clear();

        QDataStream in(&fileIn);
        QList<QGraphicsItem *> items = readItems(in);
        for (QGraphicsItem *item : items) {
            addItem(item);
        }
    }
}
