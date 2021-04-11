#include "Atom.h"

Atom::Atom(QPointF currentPos) {
    atomPos = currentPos;
    element = Carbon;
}

void Atom::addBond(Bond *p_bond){
    bonds.append(p_bond);
}

/*
void Atom::setFirstBond(Bond *p_firstBond){
    bondFirst = p_firstBond;
    numBonds += 1;
}

void Atom::setSecondBond(Bond *p_secondBond){
    bondSecond = p_secondBond;
    numBonds += 1;
}

void Atom::setThirdBond(Bond *p_thirdBond){
    bondThird = p_thirdBond;
    numBonds += 1;
}

void Atom::setFourthBond(Bond *p_fourthBond){
    bondFourth = p_fourthBond;
    numBonds += 1;
}*/

void Atom::setElement(enum Element changeElement){
    element = changeElement;
}

QPointF Atom::getPos(){
    return atomPos;
}
