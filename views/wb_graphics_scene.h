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

#ifndef WB_GRAPHICSSCENE_H
#define WB_GRAPHICSSCENE_H

#include <QObject>

class WB_GraphicsView;
class QGraphicsItem;
class QGraphicsScene;

class WB_GraphicsScene : public QObject
{
public:
    WB_GraphicsScene(WB_GraphicsView *parent);

    void saveToFile(QString const &filename);
    void loadFromFile(QString const &filename);

    QList<QGraphicsItem*> collidingItems(const QGraphicsItem *item, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const;
    QGraphicsItem *itemAt(const QPointF &position, const QTransform &deviceTransform) const;
    QList<QGraphicsItem*> topLevelItems();
    QList<QGraphicsItem*> selectedItems();
    void addItem(QGraphicsItem *item);
    void removeItem(QGraphicsItem *item);
    void update();
    void clearSelection();

private:
    QGraphicsScene *m_scene;
};

#endif // WB_GRAPHICSSCENE_H
