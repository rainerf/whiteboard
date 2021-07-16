#ifndef WHITEBOARDCOMMANDS_H
#define WHITEBOARDCOMMANDS_H

#include <QUndoCommand>

class QGraphicsScene;
class QGraphicsItem;

class PasteCommand : public QUndoCommand {
public:
    explicit PasteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem*> m_items;
    QGraphicsScene *m_scene;
};


class DeleteCommand : public QUndoCommand {
public:
    explicit DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem*> m_items;
    QGraphicsScene *m_scene;
};

#endif // WHITEBOARDCOMMANDS_H
