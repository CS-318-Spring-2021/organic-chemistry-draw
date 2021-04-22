
#include "Bond.h"
/*
Bond::Bond(){
    quality = NormalBond;
    quantity = SingleBond;
}*/
Bond::Bond(Atom *p_firstAtom, Atom *p_secondAtom){
    quality = NormalBond;
    quantity = SingleBond;
    if (p_firstAtom->bonds.size()==2){
            quality=ThickBond;
    } else if (p_firstAtom->bonds.size()==3){
            quality=DashBond;
    }

    atomFirst = p_firstAtom;
    atomSecond = p_secondAtom;
}
/*
void Bond::setFirstAtom(Atom *p_firstAtom){
    atomFirst = p_firstAtom;
}

void Bond::setSecondAtom(Atom *p_secondAtom){
    atomSecond = p_secondAtom;
}*/

void Bond::printBond(){
    printf("firsta: ");
    atomFirst->printAtom();
    printf("seconda: ");
    atomSecond->printAtom();
}
