#ifndef BOND_H
#define BOND_H

#include <molecule.h>


class atom;

class bond{
public:
    bond();

    enum BondQuality {NormalBond, ThickBond, DashBond};
    enum BondQuantity {SingleBond, DoubleBond, TripleBond};


    void setSlope(QPointF firstAtom, QPointF secondAtom);
    void setFirstAtom(atom *p_firstAtom);
    void setSecondAtom(atom *p_secondAtom);
    void changeQuantity(BondQuantity quantity);

private:
    atom *atomFirst;
    atom *atomSecond;
    double slope;
    BondQuality quality;
    BondQuantity quantity;


};

#endif // BOND_H
