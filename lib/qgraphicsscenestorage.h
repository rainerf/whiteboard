#ifndef QGRAPHICSSCENESTORAGE_H
#define QGRAPHICSSCENESTORAGE_H

#include <QDataStream>
#include <QGraphicsItem>

void saveItems(QList<QGraphicsItem *> items, QDataStream &out);
QList<QGraphicsItem *> readItems(QDataStream &in);

#endif // QGRAPHICSSCENESTORAGE_H
