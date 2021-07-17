#ifndef WHITEBOARDGRAPHICSVIEW_H
#define WHITEBOARDGRAPHICSVIEW_H

#include <QUndoStack>

#include "lib/interactive_view.h"
#include "tools/highlight_tool.h"
#include "tools/pen_tool.h"
#include "tools/pointer_tool.h"
#include "tools/text_tool.h"
#include "tools/zoom_tool.h"
#include "tools/pan_tool.h"

class WB_GraphicsView : public InteractiveView {
    Q_OBJECT

public:
    using InteractiveView::InteractiveView;

    QUndoStack * getUndoStack() {
        return &undoStack;
    }

public slots:
    void setColor(QColor const &color);
    void setPenThickness(int thickness);

    void setPenTool();
    void setTextTool();
    void setHighlightTool();
    void setPointerTool();
    void setZoomTool();
    void setPanTool();

    void deleteSelectedItems();

    void setFont(QFont const &font);
    void setFontSize(int size);

    void paste();
    void copy();

    void selectAll();
    void saveToFile(QString const &filename);
    void loadFromFile(QString const &filename);
    void clear();

signals:
    void toolInUse(bool);

private:
    void tabletEvent(QTabletEvent *event) override;

    bool m_deviceDown = false;

    PenTool penTool;
    TextTool textTool;
    HighlightTool highlightTool;
    PointerTool pointerTool;
    ZoomTool zoomTool;
    PanTool panTool;
    ToolBase *currentTool = &penTool;

    QUndoStack undoStack;
};

#endif // WHITEBOARDGRAPHICSVIEW_H
