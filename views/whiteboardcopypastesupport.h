#ifndef WHITEBOARDCOPYPASTESUPPORT_H
#define WHITEBOARDCOPYPASTESUPPORT_H

#include <QList>

class QGraphicsItem;
class QMimeData;

constexpr char MIME_TYPE[] = "application/x-wb-items";

QList<QGraphicsItem*> pasteFromMimeData(QMimeData const *mimeData);
void copyGraphicsItems(QList<QGraphicsItem*> items);

#endif // WHITEBOARDCOPYPASTESUPPORT_H
