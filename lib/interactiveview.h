#ifndef INTERACTIVEVIEW_H
#define INTERACTIVEVIEW_H

#include <QGraphicsView>

class InteractiveView : public QGraphicsView {
public:
    InteractiveView(QWidget *&);

    void pan(QPointF delta);
    void zoom(float scaleFactor);
    void zoomIn();
    void zoomOut();

    qreal getScale() const;
    void setScale(qreal);

protected:
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

private:
    // Flags to determine if zooming or panning should be done.
    bool _doMousePanning = false;
    QPoint _lastMousePos;
    qreal _scale = 1;

    qreal const _zoomDelta = 1.1;
    Qt::MouseButton const panButton = Qt::MiddleButton;

    void setMaxSize();
};

#endif // INTERACTIVEVIEW_H
