#include "qatom.h"
#include "Atom.h"
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>

QAtom::QAtom(Atom *_atom, qreal radius):atom(_atom)
{
    coop = new QGraphicsEllipseItem(-radius, -radius, 2*radius, 2*radius);
    coop->setPen(QPen(Qt::NoPen));
    coop->setBrush(QColor(0, 0, 0, 40));
    addToGroup(coop);
    setPos(atom->atomPos);

    //setAcceptHoverEvents(true);

}


void QAtom::hoverEnterEvent(QGraphicsSceneHoverEvent *){
    coop->setBrush(QColor(0, 255, 0, 48));
    printf("okay:/n");
}

void QAtom::hoverLeaveEvent(QGraphicsSceneHoverEvent *){
    coop->setBrush(QColor(0, 0, 0, 0));
    printf("less okay:/n");

}
