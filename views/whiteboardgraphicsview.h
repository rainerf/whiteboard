#ifndef WHITEBOARDGRAPHICSVIEW_H
#define WHITEBOARDGRAPHICSVIEW_H

#include "lib/interactiveview.h"
#include "tools/highlighttool.h"
#include "tools/pentool.h"
#include "tools/pointertool.h"
#include "tools/texttool.h"
#include "tools/zoomtool.h"

class WhiteBoardGraphicsView : public InteractiveView {
public:
    Q_OBJECT

    using InteractiveView::InteractiveView;

public slots:
    void setColor(QColor const &color);
    void setPenThickness(int thickness);

    void setPenTool();
    void setTextTool();
    void setHighlightTool();
    void setPointerTool();
    void setZoomTool();

    void deleteSelectedItems();

    void setFont(QFont const &font);
    void setFontSize(int size);

    void paste();
    void copy();

    void selectAll();
    void saveToFile(QString filename);
    void loadFromFile(QString filename);
    void clear();

signals:
    void toolInUse(bool);

private:
    void tabletEvent(QTabletEvent *event) override;

    QImage renderToPixmap();

    bool m_deviceDown = false;

    PenTool penTool;
    TextTool textTool;
    HighlightTool highlightTool;
    PointerTool pointerTool;
    ZoomTool zoomTool;
    WhiteBoardTool *currentTool = &penTool;
};

#endif // WHITEBOARDGRAPHICSVIEW_H
