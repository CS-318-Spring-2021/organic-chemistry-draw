#include "QAtom.h"
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
//    QLineF bond(atom->bonds[0]->atomFirst->atomPos, atom->bonds[0]->atomSecond->atomPos);
//    float sideLength = bond.length() * 0.2; //.2 is a chosen number
//    box = new QGraphicsRectItem(atom->atomPos.x()-sideLength*.5, atom->atomPos.y()+sideLength*.5, bond.length(), bond.length());
//    box->setPen(QPen(Qt::NoPen));
//    box->setBrush(QColor(255, 255, 255));
//    box->setOpacity(0.0);
//    addToGroup(box);
    //could use this to do a double click atom to make a white box over the atom area?
}

void QAtom::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    coop->setOpacity(0.25);
    //isHovering = true;
}

void QAtom::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    coop->setOpacity(0.0);
}

void QAtom::mousePressEvent(QGraphicsSceneMouseEvent *)
{}

void QAtom::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
        //box->setOpacity(1.0);
}
