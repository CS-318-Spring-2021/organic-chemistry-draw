#ifndef BOND_H
#define BOND_H

#include <molecule.h>


class atom;

class bond : public molecule{
public:
    bond();

    double slope;
    enum bondQuality {NormalBond, ThickBond, DashBond};
    enum bondQuantity {SingleBond, DoubleBond, TripleBond};
    atom *atomFirst;
    atom *atomSecond;

    void setSlope(QPointF firstAtom, QPointF secondAtom);

};

#endif // BOND_H
