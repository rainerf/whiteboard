#include "highlight_tool.h"
#include "views/wb_graphics_view.h"

void HighlightTool::setColor(QColor const &color) {
    QColor color_ = QColor::fromHsv(color.hue(), color.saturation() / 5, color.value());
    PenTool::setColor(color_);
}

void HighlightTool::setPen(int thickness) {
    PenTool::setPen(thickness * 10);
}

WB_ItemGroup *HighlightTool::createGroup() {
    auto *group = PenTool::createGroup();
    group->setZValue(-1);
    return group;
}
