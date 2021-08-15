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

#include "pen_tool.h"
#include "views/wb_graphics_view.h"
#include "views/wb_graphics_scene.h"

#include <QUndoCommand>
#include <QtMath>


namespace detail {
int constexpr SMOOTHING_LENGTH = 5;

template <typename T>
T weightedMean(QList<T> &list, int index) {
    std::array<int, SMOOTHING_LENGTH> weights = {1, 3, 7, 3, 1};
    int const totalWeight = std::accumulate(begin(weights), end(weights), 0);

    T sum{};
    for (int i=0; i<static_cast<int>(weights.size()); ++i) {
        sum += list[qBound(0, index+i-2, list.length()-1)] * weights[i];
    }
    return sum/totalWeight;
}

template <typename T>
void updateWithMovingAverage(QList<T> &list, int i) {
    list[i] = weightedMean(list, i);
}

}

class PenTool::DrawCommand : public QUndoCommand {
public:
    explicit DrawCommand(QPointF initialPoint, QColor color, WB_ItemGroup *group, WB_GraphicsScene *graphicsScene, QUndoCommand *parent = nullptr) :
            QUndoCommand("Draw", parent),
            m_color(color),
            m_group(group),
            m_scene(graphicsScene) {
        m_points.append(initialPoint);
    }

    void undo() override {
        m_scene->removeItem(m_group);
    }

    void redo() override {
        m_scene->addItem(m_group);
    }

    void addPoint(QPointF toPoint, qreal thickness) {
        if ((toPoint - m_points.last()).manhattanLength() < 1)
            return;

        auto *path = new QGraphicsPathItem();
        setPathItemTo(path, m_points.last(), toPoint, thickness);
        m_group->addToGroup(path);

        m_items.append(path);
        m_points.append(toPoint);
        m_thicknesses.append(thickness);

        if (m_points.length() >= qCeil(detail::SMOOTHING_LENGTH/2.)) {
            int const currentIndex = m_points.length()-qCeil(detail::SMOOTHING_LENGTH/2.);
            detail::updateWithMovingAverage(m_points, currentIndex);

            // if this is the point #0, we don't have a path to correct yet (the first path
            // obviously goes from points #0 to #1); so, after we've corrected it, there's
            // nothing more to do but wait until the next point, where the segment #0 -- #1
            // will be adjusted
            if (currentIndex == 0)
                return;

            detail::updateWithMovingAverage(m_thicknesses, currentIndex-1);

            setPathItemTo(m_items[currentIndex-1], m_points[currentIndex-1], m_points[currentIndex], m_thicknesses[currentIndex-1]);
        }
    }

    void finalize() {
        // we need to adjust the last points
        if (m_points.length() > qCeil(detail::SMOOTHING_LENGTH/2.)) {
            for (int currentIndex=m_points.length()-qFloor(detail::SMOOTHING_LENGTH/2.); currentIndex<m_points.length(); ++currentIndex) {
                detail::updateWithMovingAverage(m_points, currentIndex);
                detail::updateWithMovingAverage(m_thicknesses, currentIndex-1);

                setPathItemTo(m_items[currentIndex-1], m_points[currentIndex-1], m_points[currentIndex], m_thicknesses[currentIndex-1]);
            }
        }
    }

private:
    void setPathItemTo(QGraphicsPathItem *item, QPointF const &from, QPointF const &to, qreal thickness) {
        auto const pen = QPen(m_color, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        item->setPen(pen);

        QPainterPath p;
        p.moveTo(from);
        p.lineTo(to);
        item->setPath(p);
    }

    QColor const m_color;
    WB_ItemGroup *m_group;
    WB_GraphicsScene *m_scene;
    QList<QPointF> m_points;
    QList<qreal> m_thicknesses;
    QList<QGraphicsPathItem*> m_items;
};

void PenTool::setColor(const QColor &color) {
    m_color = color;
}

void PenTool::setPen(int thickness) {
    m_maxThickness = thickness;
}

bool PenTool::handleTabletPress(WB_GraphicsView &view, QTabletEvent &event) {
    m_currentCommand = new DrawCommand(view.mapToScene(event.pos()), m_color, createGroup(), view.scene());
    view.getUndoStack()->push(m_currentCommand);
    return true;
}

void PenTool::handleTabletMove(WB_GraphicsView &view, QTabletEvent &event) {
    m_currentCommand->addPoint(view.mapToScene(event.pos()), event.pressure() * m_maxThickness*3/4 + m_maxThickness/4);
}

void PenTool::handleTabletRelease(WB_GraphicsView & /*view*/, QTabletEvent &/*event*/) {
    m_currentCommand->finalize();
}

WB_ItemGroup *PenTool::createGroup() {
    auto *group = new WB_ItemGroup();
    group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    return group;
}
