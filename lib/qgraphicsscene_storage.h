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
