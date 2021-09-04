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

#ifndef INTERACTIVEVIEW_H
#define INTERACTIVEVIEW_H

#include <QGraphicsView>

class InteractiveView : public QGraphicsView {
public:
    InteractiveView(QWidget *&);

    void pan(QPoint delta);
    void zoomIn();
    void zoomOut();
    void zoomToFit();
    void setZoom(qreal);

protected:
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

private:
    // Flags to determine if zooming or panning should be done.
    bool _doMousePanning = false;
    QPoint _lastMousePos;

    qreal const _zoomDelta = 1.1;
    Qt::MouseButton const panButton = Qt::MiddleButton;

    void setMaxSize();
    void zoomDelta(float scaleFactor);
};

#endif // INTERACTIVEVIEW_H
