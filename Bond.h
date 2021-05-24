#ifndef BOND_H
#define BOND_H

#include "Molecule.h"
#include <QPointF>
#include <QVector>

class Atom;

class Bond{
public:
    Bond(Atom *p_firstAtom, Atom *p_secondAtom);

    enum {NormalBond, ThickBond, DashBond, DoubleBond, TripleBond, n};

    Atom *atomFirst;
    Atom *atomSecond;
    int drawStyle;
    int quantity;
    void changeBond();
};

#endif // BOND_H
