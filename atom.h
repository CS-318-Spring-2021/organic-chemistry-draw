#ifndef ATOM_H
#define ATOM_H

#include <molecule.h>
#include <drawspace.h>

class bond;

class atom : public molecule{
public:
    atom(QPoint lastAtom);
    atom(QPoint currentAtom, QPointF nextAtom);

    QPointF atomPos;
    enum element {Carbon, Hygrogen, Nitrogen, Oxygen, Flourine, Sodium, Magnesium, Chlorine, Phosphorus, Sulfur, Potasium, Calcium, Bromine};
    enum order {First, Middle, Last};
    double const atomRadius = 10; //need to decide this later
    int numBonds = 0;
    bond *bondFirst;
    bond *bondSecond;
    bond *bondThird;
    bond *bondFourth;

    void setAtomPos(QPointF &pos);
    void addBond(QPointF vertex1, QPointF vertex2);
};

#endif // ATOM_H
