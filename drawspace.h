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
class DrawnObject;
class Molecule;

class drawspace : public QGraphicsView
{
    Q_OBJECT


    QPointF lastPos;
    bool mouseIsDown;



public:
    enum {MousePressed, MouseReleased, MouseMoved};
    QGraphicsScene mScene;
    DrawnObject *currentDrawnObject;
    DrawnObject *freehandObject;
    bool recording;
    int appending = -1;
    QVector<Molecule*> molecules;
    QVector<QVector<Molecule*>> undoStack;
    drawspace();
    void maybeAddSegment(const QPointF &pos); 
    void replaceSegment(const QPointF &firstPos, const QPointF &lastPos);
    void drawExisting();
    QVector<Molecule*> makeMoleculesFreshCopy();


protected:
    virtual void mousePressEvent(QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseMoveEvent(QMouseEvent *evt) override;
};

#endif // DRAWSPACE_H
