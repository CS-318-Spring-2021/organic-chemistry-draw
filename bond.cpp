#include "bond.h"

bond::bond(){
    quality = NormalBond;
    quantity = SingleBond;
}

void bond::setFirstAtom(atom *p_firstAtom){
    atomFirst = p_firstAtom;
}

void bond::setSecondAtom(atom *p_secondAtom){
    atomSecond = p_secondAtom;
}
