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

#ifndef WHITEBOARDITEM_H
#define WHITEBOARDITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

template <typename BASE>
class WB_Item : public BASE {
public:

    template<typename... Args>
    explicit WB_Item(Args&&... args): BASE(std::forward<Args>(args)...) {
        this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QStyleOptionGraphicsItem unselectedOption = *option;
        unselectedOption.state &= ~QStyle::State_Selected;
        BASE::paint(painter, &unselectedOption, widget);

        updateHandlePos();

        if (option->state & QStyle::State_Selected) {
            auto const originalBoundingRect = BASE::boundingRect();
            QPen pen(QColor(0, 0, 200));
            pen.setCosmetic(true);
            painter->setPen(pen);
            painter->drawRect(originalBoundingRect);

            painter->setBrush(QColor(0, 0, 200));
            painter->drawEllipse(m_handlePos);
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        if (m_resizing) {
            auto const newWidth = std::max(this->mapToScene(event->pos()).x() - (this->x() + boundingRect().left()), this->originalWidth()/10.);
            this->setTransformOriginPoint(boundingRect().topLeft());
            this->prepareGeometryChange();
            this->setZoom(newWidth/this->originalWidth());
        } else
            BASE::mouseMoveEvent(event);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        if (m_handlePos.contains(event->pos())) {
            m_resizePos = event->pos();
            m_resizing = true;
        }

        BASE::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        BASE::mouseReleaseEvent(event);
        m_resizing = false;
    }

    QRectF boundingRect() const override {
        return BASE::boundingRect().adjusted(0, 0, 5, 5);
    }

    bool contains(const QPointF &point) const override {
        return boundingRect().contains(point);
    }

    QPainterPath shape() const override {
        QPainterPath p;
        p.addRect(boundingRect());
        return p;
    }

    void updateHandlePos() {
        m_handlePos = QRectF(boundingRect().right() - 10, boundingRect().bottom() - 10, 10, 10);
    }

private:
    QRectF m_handlePos;
    QPointF m_resizePos;
    bool m_resizing {false};
};

#endif // WHITEBOARDITEM_H
