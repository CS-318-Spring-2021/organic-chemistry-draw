#ifndef DRAWSPACE_H
#define DRAWSPACE_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>
#include <cmath>

class QGraphicsPathItem;

class drawspace : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene mScene;
    QPointF lastPos;
    QGraphicsPathItem *crosshairs;

    void maybeAddSegment(const QPointF &pos);

public:
    enum {MousePressed, MouseReleased, MouseMoved};
    enum MoleculeType {Cyclic, Linear};

    drawspace();

    void setCrosshairsVisible(bool visible);
    void setCrosshairsPos(const QPointF &pos);

protected:
    virtual void mousePressEvent(QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseMoveEvent(QMouseEvent *evt) override;

signals:
    void mouseEvent(int type, int when, QPointF pos);
};

#endif // DRAWSPACE_H
