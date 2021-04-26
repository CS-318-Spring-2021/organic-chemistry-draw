#include "Atom.h"

Atom::Atom(QPointF currentPos) {
    atomPos = currentPos;
    element = Carbon;
}

void Atom::addBond(Bond *p_bond){
    //count neighbor bonds, calculate what bond we want, change neighbor bonds
    bonds.append(p_bond);
    if(this->getBonds()==4){
        bonds[2]->quality = Bond::ThickBond;
        bonds[3]->quality = Bond::DashBond;
    }
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

int Atom::getBonds(){
    return bonds.size();
}
