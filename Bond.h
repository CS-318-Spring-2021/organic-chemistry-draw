#ifndef BOND_H
#define BOND_H

#include "molecule.h"

class Atom;

class Bond{
public:
    Bond();

    enum BondQuality {NormalBond, ThickBond, DashBond};
    enum BondQuantity {SingleBond, DoubleBond, TripleBond};


    void setSlope(QPointF firstAtom, QPointF secondAtom);
    void setFirstAtom(Atom *p_firstAtom);
    void setSecondAtom(Atom *p_secondAtom);
    void changeQuantity(BondQuantity quantity);

private:
    Atom *atomFirst;
    Atom *atomSecond;
    double slope;
    BondQuality quality;
    BondQuantity quantity;


};

#endif // BOND_H
