#include "atom.h"

atom::atom(QPointF currentPos) {
    atomPos = currentPos;
    element = Carbon;
}

void atom::setFirstBond(bond *p_firstBond){
    bondFirst = p_firstBond;
    numBonds += 1;
}

void atom::setSecondBond(bond *p_secondBond){
    bondSecond = p_secondBond;
    numBonds += 1;
}

void atom::setThirdBond(bond *p_thirdBond){
    bondThird = p_thirdBond;
    numBonds += 1;
}

void atom::setFourthBond(bond *p_fourthBond){
    bondFourth = p_fourthBond;
    numBonds += 1;
}

void atom::setElement(enum Element changeElement){
    element = changeElement;
}
