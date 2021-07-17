#ifndef HIGHLIGHTTOOL_H
#define HIGHLIGHTTOOL_H

#include "pen_tool.h"

class HighlightTool : public PenTool {
public:
    void setColor(QColor const &color) override;
    void setPen(int thickness) override;

    WB_ItemGroup *createGroup() override;
};

#endif // HIGHLIGHTTOOL_H
