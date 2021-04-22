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

QPointF Atom::getPos(){
    return atomPos;
}

void Atom::setAtomPos(QPointF newPoint){
    atomPos = newPoint;
}

QPointF Atom::getAtomPos(){
   return atomPos;
}

void Atom::printAtom(){
    printf("(%i, %i)\n", int(atomPos.x()), int(atomPos.y()));
}

void Atom::printAtomDetailed(){
    printf("neighbors: %i\n", bonds.size());
    for (int i=0; i<bonds.size();i++){
        bonds[i]->printBond();
    }
}
