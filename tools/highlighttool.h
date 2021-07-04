#ifndef HIGHLIGHTTOOL_H
#define HIGHLIGHTTOOL_H

#include <QGraphicsView>

#include "pentool.h"

class HighlightTool : public PenTool {
public:
    void setColor(QColor const &color) override;
    void setPen(int thickness) override;

    void handleTabletPress(QGraphicsView &view, QTabletEvent &event) override;
};

#endif // HIGHLIGHTTOOL_H
