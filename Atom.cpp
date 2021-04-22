#include "Atom.h"

Atom::Atom(QPointF currentPos) {
    atomPos = currentPos;
    element = Carbon;
}

void Atom::addBond(Bond *p_bond){
    bonds.append(p_bond);
}

void Atom::setElement(enum Element changeElement){
    element = changeElement;
}

void Atom::setAtomPos(QPointF newPoint){
    atomPos = newPoint;
}
