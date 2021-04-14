#include "Bond.h"

Bond::Bond(){
    quality = NormalBond;
    quantity = SingleBond;
}

void Bond::setFirstAtom(Atom *p_firstAtom){
    atomFirst = p_firstAtom;
}

void Bond::setSecondAtom(Atom *p_secondAtom){
    atomSecond = p_secondAtom;
}
