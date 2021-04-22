#ifndef QATOM_H
#define QATOM_H

#include <QGraphicsItemGroup>

class Atom;
class QAtom : public QGraphicsItemGroup
{
public:
    QAtom(Atom *_atom, qreal radius);
    QGraphicsEllipseItem *coop;
    Atom *atom;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
};

#endif // QATOM_H
