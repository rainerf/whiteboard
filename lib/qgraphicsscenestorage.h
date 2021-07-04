#ifndef QGRAPHICSSCENESTORAGE_H
#define QGRAPHICSSCENESTORAGE_H

#include <QDataStream>
#include <QGraphicsItem>

class FileError : public std::exception {};
class FileFormatError : public FileError {};
class FileVersionError : public FileError {};

void saveItems(QList<QGraphicsItem *> items, QDataStream &out);
QList<QGraphicsItem *> readItems(QDataStream &in);

#endif // QGRAPHICSSCENESTORAGE_H
