#ifndef QATOM_H
#define QATOM_H

#include <QGraphicsItemGroup>

class Atom;
class QAtom : public QGraphicsItemGroup
{
public:
    QAtom(Atom *_atom, qreal radius);
    QGraphicsEllipseItem *coop;
    QGraphicsRectItem * box; //to be used to perhaps draw a box over the atom
    Atom *atom;

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *evt);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // QATOM_H
