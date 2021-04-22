#include "qbond.h"
#include "Bond.h"
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>

QBond::QBond(Bond *_bond):bond(_bond)
{
    qgline = new QGraphicsLineItem(QLineF(bond->atomFirst->atomPos, bond->atomSecond->atomPos));
    /*
    coop = new QGraphicsEllipseItem(-radius, -radius, 2*radius, 2*radius);
    coop->setPen(QPen(Qt::NoPen));
    coop->setBrush(QColor(0, 0, 0, 40));
    addToGroup(coop);
    setPos(atom->atomPos);

    //setAcceptHoverEvents(true);
    */
}

/*
void QAtom::hoverEnterEvent(QGraphicsSceneHoverEvent *){
    coop->setBrush(QColor(0, 255, 0, 48));
    printf("okay:/n");
}

void QAtom::hoverLeaveEvent(QGraphicsSceneHoverEvent *){
    coop->setBrush(QColor(0, 0, 0, 0));
    printf("less okay:/n");

}*/
