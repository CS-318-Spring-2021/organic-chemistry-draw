
#include "Bond.h"

Bond::Bond(Atom *p_firstAtom, Atom *p_secondAtom){
    quality = NormalBond;
    quantity = SingleBond;

    atomFirst = p_firstAtom;
    atomSecond = p_secondAtom;
}
