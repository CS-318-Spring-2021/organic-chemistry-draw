#ifndef DRAWSPACE_H
#define DRAWSPACE_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>

class QGraphicsPathItem;

class drawspace : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene mScene;
    QPointF lastPos;
    QGraphicsPathItem *crosshairs;



public:
    enum { MousePressed, MouseReleased, MouseMoved };

    drawspace();

    void setCrosshairsVisible(bool visible);
    void setCrosshairsPos(const QPointF &pos);
    void maybeAddSegment(const QPointF &pos);
    void replaceSegment(const QPointF &firstPos, const QPointF &lastPos);

protected:
    virtual void mousePressEvent(QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseMoveEvent(QMouseEvent *evt) override;

signals:
    void mouseEvent(int type, int when, QPointF pos);
};

#endif // DRAWSPACE_H
