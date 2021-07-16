#ifndef WHITEBOARDGRAPHICSVIEW_H
#define WHITEBOARDGRAPHICSVIEW_H

#include <QUndoStack>

#include "lib/interactiveview.h"
#include "tools/highlighttool.h"
#include "tools/pentool.h"
#include "tools/pointertool.h"
#include "tools/texttool.h"
#include "tools/zoomtool.h"
#include "tools/pantool.h"

class WhiteBoardGraphicsView : public InteractiveView {
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

    QImage renderToPixmap();

    bool m_deviceDown = false;

    PenTool penTool;
    TextTool textTool;
    HighlightTool highlightTool;
    PointerTool pointerTool;
    ZoomTool zoomTool;
    PanTool panTool;
    WhiteBoardTool *currentTool = &penTool;

    QUndoStack undoStack;
};

#endif // WHITEBOARDGRAPHICSVIEW_H
