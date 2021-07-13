#include "whiteboardgraphicsview.h"

#include <QApplication>
#include <QClipboard>
#include <QDataStream>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include <QMimeData>
#include <QPixmap>
#include <QSettings>
#include <QTabletEvent>
#include <QtMath>
#include <QMessageBox>

#include "items/whiteboardtextitem.h"
#include "items/whiteboardpixmapitem.h"
#include "lib/qgraphicsscenestorage.h"

void WhiteBoardGraphicsView::tabletEvent(QTabletEvent *event) {
    switch (event->type()) {
    case QEvent::TabletPress:
        // take the focus from other widgets (which Qt doesn't do for some reason),
        // and unfocus all items in the scene
        setFocus();
        for (auto &&i : scene()->items())
            i->clearFocus();

        if (!m_deviceDown) {
            m_deviceDown = true;
            currentTool->handleTabletPress(*this, *event);
            emit toolInUse(true);
        }
        break;
    case QEvent::TabletMove:
        if (m_deviceDown) {
            currentTool->handleTabletMove(*this, *event);
        }
        break;
    case QEvent::TabletRelease:
        if (m_deviceDown && event->buttons() == Qt::NoButton) {
            m_deviceDown = false;
            currentTool->handleTabletRelease(*this, *event);
            emit toolInUse(false);
        }
        break;
    default:
        break;
    }
    event->accept();
}

void WhiteBoardGraphicsView::setColor(const QColor &color) {
    penTool.setColor(color);
    textTool.setColor(color);
    highlightTool.setColor(color);
}

void WhiteBoardGraphicsView::setPenThickness(int thickness) {
    penTool.setPen(thickness);
    highlightTool.setPen(thickness);
}

void WhiteBoardGraphicsView::setPenTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &penTool;
}

void WhiteBoardGraphicsView::setTextTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &textTool;
}

void WhiteBoardGraphicsView::setHighlightTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &highlightTool;
}

void WhiteBoardGraphicsView::setPointerTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &pointerTool;
}

void WhiteBoardGraphicsView::setZoomTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &zoomTool;
}

void WhiteBoardGraphicsView::setPanTool() {
    if (m_deviceDown)
        throw std::logic_error("Changing a tool while it's in use should not be possible!");
    currentTool = &panTool;
}

void WhiteBoardGraphicsView::deleteSelectedItems() {
    for (auto &&i : scene()->selectedItems()) {
        scene()->removeItem(i);
    }
}

void WhiteBoardGraphicsView::setFont(const QFont &font) {
    textTool.setFont(font);
}

void WhiteBoardGraphicsView::setFontSize(int size) {
    textTool.setFontSize(size);
}

void WhiteBoardGraphicsView::paste() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasImage()) {
        auto *item = new WhiteBoardPixmapItem(qvariant_cast<QPixmap>(mimeData->imageData()));
        scene()->addItem(item);
    } else if (mimeData->hasHtml()) {
        auto *textItem = new WhiteBoardTextItem();
        textItem->setHtml(mimeData->html());
        scene()->addItem(textItem);
    } else if (mimeData->hasText()) {
        auto *textItem = new WhiteBoardTextItem();
        textItem->setPlainText(mimeData->text());
        scene()->addItem(textItem);
    }
}

QImage WhiteBoardGraphicsView::renderToPixmap() {
    auto const boundingRect = scene()->itemsBoundingRect();
    int width = qCeil(boundingRect.width());
    int height = qCeil(boundingRect.height());
    QImage pixmap(width * 8, height * 8, QImage::Format_ARGB32);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    scene()->render(&painter, pixmap.rect(), boundingRect);
    return pixmap;
}

void WhiteBoardGraphicsView::copy() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(renderToPixmap());
}

void WhiteBoardGraphicsView::selectAll() {
    for (auto &&i : scene()->items())
        i->setSelected(true);
}

void WhiteBoardGraphicsView::saveToFile(QString const &filename) {
    QFile fileOut(filename);
    if (fileOut.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileOut);
        saveItems(scene()->items(), out);
        fileOut.close();
    }
}

void WhiteBoardGraphicsView::loadFromFile(QString const &filename) {
    QFile fileIn(filename);
    if (fileIn.open(QIODevice::ReadOnly)) {
        scene()->clear();

        QDataStream in(&fileIn);
        try {
            QList<QGraphicsItem *> items = readItems(in);
            for (QGraphicsItem *item : items) {
                scene()->addItem(item);
            }
        } catch (FileError &) {
            QMessageBox::critical(this, "Whiteboard", "File format incorrect!");
        }
    }
}

void WhiteBoardGraphicsView::clear() {
    scene()->clear();
}
