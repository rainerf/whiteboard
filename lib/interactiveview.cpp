#include <QKeyEvent>
#include <QWheelEvent>

#include "interactiveview.h"

InteractiveView::InteractiveView(QWidget *&parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMaxSize();
    centerOn(0, 0);

    setDragMode(QGraphicsView::RubberBandDrag);
}

void InteractiveView::setScale(qreal newScale) {
    setTransform(QTransform() * newScale);
}

void InteractiveView::zoomToFit() {
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void InteractiveView::mouseMoveEvent(QMouseEvent *event) {
    if (_doMousePanning) {
        QPointF mouseDelta = event->pos() - _lastMousePos;
        pan(mouseDelta, true);
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

void InteractiveView::zoom(float scaleFactor) {
    scale(scaleFactor, scaleFactor);
}

void InteractiveView::zoomIn() {
    zoom(_zoomDelta);
}

void InteractiveView::zoomOut() {
    zoom(1 / _zoomDelta);
}

void InteractiveView::pan(QPointF delta, bool mouse) {
    if (mouse) {
        // Have panning be anchored from the mouse.
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    }

    auto const view_width = viewport()->rect().width();
    auto const view_height = viewport()->rect().height();
    QPoint newCenter(view_width / 2 - delta.x(), view_height / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // For zooming to anchor from the view center.
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
