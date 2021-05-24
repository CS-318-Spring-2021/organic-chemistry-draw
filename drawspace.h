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
    float bondLength = -1;
    QVector<Molecule*> molecules;
    QVector<DrawnObject*> freeHandObjects;
    QVector<QVector<Molecule*>> undoStackMolecule;
    QVector<QVector<DrawnObject*>> undoStackDrawnObject;

    drawspace();
    void maybeAddSegment(const QPointF &pos);
    void drawExisting();
    QVector<Molecule*> makeMoleculesFreshCopy();
    QVector<DrawnObject*> makeDrawnObjectsFreshCopy();

protected:
    virtual void mousePressEvent(QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseMoveEvent(QMouseEvent *evt) override;
};

#endif // DRAWSPACE_H
