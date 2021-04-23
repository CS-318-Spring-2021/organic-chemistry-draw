#include "Atom.h"

Atom::Atom(QPointF currentPos) {
    atomPos = currentPos;
    element = Carbon;
}

void Atom::addBond(Bond *p_bond){
    bonds.append(p_bond);
}

void Atom::removeBond(Bond *p_bond){
    for (int i=0; i<bonds.size(); i++){
        if (bonds[i]==p_bond){
            bonds.remove(i);
            return;
        }
    }
}

void Atom::setElement(enum Element changeElement){
    element = changeElement;
}

void Atom::setAtomPos(QPointF newPoint){
    atomPos = newPoint;
}
