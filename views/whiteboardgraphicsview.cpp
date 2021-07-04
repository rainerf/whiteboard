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

#include "whiteboardgraphicsview.h"
#include "items/whiteboardtextitem.h"
#include "lib/qgraphicsscenestorage.h"

void WhiteBoardGraphicsView::tabletEvent(QTabletEvent *event) {
    switch (event->type()) {
    case QEvent::TabletPress:
        setFocus();
        for (auto &&i : scene()->items())
            i->clearFocus();

        if (!m_deviceDown) {
            m_deviceDown = true;
            currentTool->handleTabletPress(*this, *event);
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
    currentTool = &penTool;
}

void WhiteBoardGraphicsView::setTextTool() {
    currentTool = &textTool;
}

void WhiteBoardGraphicsView::setHighlightTool() {
    currentTool = &highlightTool;
}

void WhiteBoardGraphicsView::setPointerTool() {
    currentTool = &pointerTool;
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
        auto item = new QGraphicsPixmapItem(qvariant_cast<QPixmap>(mimeData->imageData()));
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        scene()->addItem(item);
    } else if (mimeData->hasHtml()) {
        auto textItem = new WhiteBoardTextItem();
        textItem->setHtml(mimeData->html());
        scene()->addItem(textItem);
    } else if (mimeData->hasText()) {
        auto textItem = new WhiteBoardTextItem();
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

void WhiteBoardGraphicsView::saveToFile() {
    QFile fileOut("/home/rainer/file.txt");
    if (fileOut.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileOut);
        saveItems(scene()->items(), out);
        fileOut.close();
    }
}

void WhiteBoardGraphicsView::loadFromFile() {
    QFile fileIn("/home/rainer/file.txt");
    if (fileIn.open(QIODevice::ReadOnly)) {
        QDataStream in(&fileIn);
        QList<QGraphicsItem *> items = readItems(in);
        for (QGraphicsItem *item : items) {
            scene()->addItem(item);
        }
    }
}

void WhiteBoardGraphicsView::clear() {
    scene()->clear();
}
