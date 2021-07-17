#ifndef WB_GRAPHICSSCENE_H
#define WB_GRAPHICSSCENE_H

#include <QGraphicsScene>

class WB_GraphicsScene : public QGraphicsScene
{
public:
    WB_GraphicsScene(QWidget *parent);

    void saveToFile(QString const &filename);
    void loadFromFile(QString const &filename);
};

#endif // WB_GRAPHICSSCENE_H
