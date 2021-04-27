
#include "Bond.h"

Bond::Bond(Atom *p_firstAtom, Atom *p_secondAtom){
    drawStyle = NormalBond; //0, 1, 2
    quantity = 1; //integers for 1, 2, and 3



    atomFirst = p_firstAtom;
    atomSecond = p_secondAtom;
}

void Bond::changeBond(){
    //iterate through drawStyle +=1




}

//TODO: set angle? or within atom class...
