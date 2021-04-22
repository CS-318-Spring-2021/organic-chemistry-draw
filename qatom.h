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
};

#endif // QATOM_H
