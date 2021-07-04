#include "highlighttool.h"

void HighlightTool::setColor(QColor const &color) {
    QColor color_ = QColor::fromHsv(color.hue(), color.saturation() / 5, color.value());
    PenTool::setColor(color_);
}

void HighlightTool::setPen(int thickness) {
    PenTool::setPen(thickness * 10);
}

void HighlightTool::handleTabletPress(QGraphicsView &view, QTabletEvent &event) {
    PenTool::handleTabletPress(view, event);
    currentGroup()->setZValue(-1);
}
