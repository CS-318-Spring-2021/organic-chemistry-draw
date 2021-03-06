#include "Atom.h"

Atom::Atom(QPointF currentPos) {
    atomPos = currentPos;
    element = Carbon;
}

void Atom::addBond(Bond *p_bond){
    //count neighbor bonds, calculate what bond we want, change neighbor bonds
    bonds.append(p_bond);
    /*
    if(this->getNumBonds()==4){
        bonds[2]->drawStyle = Bond::ThickBond;
        bonds[3]->drawStyle = Bond::DashBond;
    }*/
    //could be used to set the next 2 bonds off of an atom with 2 bonds already
    //to be thick and dashed.
}

void Atom::removeBond(Bond *p_bond){
    for (int i=0; i<bonds.size(); i++){
        if (bonds[i]==p_bond){
            bonds.remove(i);
            return;
        }
    }
}

//could be used to change the element type, but is currently unused
void Atom::setElement(enum Element changeElement){
    element = changeElement;
}

void Atom::setAtomPos(QPointF newPoint){
    atomPos = newPoint;
}

int Atom::getNumBonds(){
    return bonds.size();
}
