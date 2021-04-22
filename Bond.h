#ifndef BOND_H
#define BOND_H

#include "Molecule.h"
#include <QPointF>
#include <QVector>

class Atom;

class Bond{
public:
    //Bond();
    Bond(Atom *p_firstAtom, Atom *p_secondAtom);

    enum BondQuality {NormalBond, ThickBond, DashBond};
    enum BondQuantity {SingleBond, DoubleBond, TripleBond};


    //void setSlope(QPointF firstAtom, QPointF secondAtom);
    //void setFirstAtom(Atom *p_firstAtom);
    //void setSecondAtom(Atom *p_secondAtom);
    //void setQuantity(BondQuantity quantity);
    void setQuality(BondQuality quality);
    Atom *atomFirst;
    Atom *atomSecond;
    //double slope;
    BondQuality quality;
    BondQuantity quantity;
    void printBond();


};

#endif // BOND_H
