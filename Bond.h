#ifndef BOND_H
#define BOND_H

#include "Molecule.h"
#include <QPointF>
#include <QVector>

class Atom;

class Bond{
public:
    Bond(Atom *p_firstAtom, Atom *p_secondAtom);

    enum BondDrawStyle {NormalBond, ThickBond, DashBond, DoubleBond, TripleBond};


    Atom *atomFirst;
    Atom *atomSecond;
    BondDrawStyle drawStyle;
    int quantity;
    void changeBond();

};

#endif // BOND_H
