#ifndef DRAWSPACE_H
#define DRAWSPACE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QObject>
#include <QWidget>
#include <cmath>

class QGraphicsPathItem;
class QAtom;
class QBond;
class drawspace : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene mScene;
    QPointF lastPos;
    QGraphicsPathItem *crosshairs;
    bool mouseIsDown;


public:
    enum {MousePressed, MouseReleased, MouseMoved};

    drawspace();
    void maybeAddSegment(const QPointF &pos); 
    QGraphicsItem* getItem(QPoint pos);
    QMap<void*, void*> locationMap;

    void setCrosshairsVisible(bool visible);
    void setCrosshairsPos(const QPointF &pos);
    void replaceSegment(const QPointF &firstPos, const QPointF &lastPos);
    void drawAtom(QAtom *qatom);
    void drawBond(QBond *qbond);


protected:
    virtual void mousePressEvent(QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseMoveEvent(QMouseEvent *evt) override;


signals:
    void mouseEvent(int type, int when, QPointF pos);
};

#endif // DRAWSPACE_H
