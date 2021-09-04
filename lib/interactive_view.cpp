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

#include "interactive_view.h"

#include <QKeyEvent>
#include <QWheelEvent>
#include <QScrollBar>

InteractiveView::InteractiveView(QWidget *&parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMaxSize();
    centerOn(0, 0);

    setDragMode(QGraphicsView::RubberBandDrag);
}

void InteractiveView::setZoom(qreal newScale) {
    setTransform(QTransform() * newScale);
}

void InteractiveView::zoomToFit() {
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void InteractiveView::mouseMoveEvent(QMouseEvent *event) {
    if (_doMousePanning) {
        QPoint mouseDelta = event->pos() - _lastMousePos;
        pan(mouseDelta);
    }

    QGraphicsView::mouseMoveEvent(event);
    _lastMousePos = event->pos();
}

void InteractiveView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == panButton) {
        _lastMousePos = event->pos();
        _doMousePanning = true;
        setDragMode(QGraphicsView::NoDrag);
    }

    QGraphicsView::mousePressEvent(event);
}

void InteractiveView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == panButton) {
        _doMousePanning = false;
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void InteractiveView::wheelEvent(QWheelEvent *event) {
    QPoint scrollAmount = event->angleDelta();

    // Apply zoom.
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void InteractiveView::setMaxSize() {
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
}

void InteractiveView::zoomDelta(float scaleFactor) {
    scale(scaleFactor, scaleFactor);
}

void InteractiveView::zoomIn() {
    zoomDelta(_zoomDelta);
}

void InteractiveView::zoomOut() {
    zoomDelta(1 / _zoomDelta);
}

void InteractiveView::pan(QPoint delta) {
    QScrollBar * const xBar=horizontalScrollBar();
    QScrollBar * const yBar=verticalScrollBar();
    xBar->setValue(xBar->value() - delta.x());
    yBar->setValue(yBar->value() - delta.y());
}
