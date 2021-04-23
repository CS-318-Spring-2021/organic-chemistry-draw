
#include "Bond.h"

Bond::Bond(Atom *p_firstAtom, Atom *p_secondAtom){
    quality = NormalBond;
    quantity = SingleBond;

    if (p_firstAtom->bonds.size()==2){ //needs to be fixed up here this is just kinda making things up
            quality=ThickBond;
    } else if (p_firstAtom->bonds.size()==3){
            quality=DashBond;
    }

    atomFirst = p_firstAtom;
    atomSecond = p_secondAtom;
}
