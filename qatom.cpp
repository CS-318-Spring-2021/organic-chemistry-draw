#include "qatom.h"
#include "Atom.h"
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>

QAtom::QAtom(Atom *_atom, qreal radius):atom(_atom)
{
    coop = new QGraphicsEllipseItem(-radius, -radius, 2*radius, 2*radius);
    coop->setPen(QPen(Qt::NoPen));
    coop->setBrush(QColor(0, 0, 0));
    coop->setOpacity(0.0);
    addToGroup(coop);
    setPos(atom->atomPos);
    setAcceptHoverEvents(true);
}

void QAtom::hoverEnterEvent(QGraphicsSceneHoverEvent *evt) {
    coop->setOpacity(0.25);
}

void QAtom::hoverLeaveEvent(QGraphicsSceneHoverEvent *evt) {
    coop->setOpacity(0.0);
}
