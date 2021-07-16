#include "highlighttool.h"
#include "views/whiteboardgraphicsview.h"

void HighlightTool::setColor(QColor const &color) {
    QColor color_ = QColor::fromHsv(color.hue(), color.saturation() / 5, color.value());
    PenTool::setColor(color_);
}

void HighlightTool::setPen(int thickness) {
    PenTool::setPen(thickness * 10);
}

WhiteBoardItemGroup *HighlightTool::createGroup() {
    auto *group = PenTool::createGroup();
    group->setZValue(-1);
    return group;
}
