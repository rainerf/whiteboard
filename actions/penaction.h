#ifndef PENACTION_H
#define PENACTION_H

#include <QAction>
#include <QPixmap>

class PenAction : public QAction {
    Q_OBJECT

public:
    PenAction(int thickness, QObject *parent = nullptr);

signals:
    void penSelected(int thickness);

private:
    int m_thickness;

    QPixmap createIcon() const;
};

#endif // PENACTION_H
