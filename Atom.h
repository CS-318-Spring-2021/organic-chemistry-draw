#ifndef ATOM_H
#define ATOM_H

#include "Molecule.h"
#include <QPointF>
#include <QVector>

class Bond;

class Atom{
public:
    Atom(QPointF currentPos);

    //unused but could be helpful in future
    enum Element {
        Carbon,
        Hygrogen,
        Nitrogen,
        Oxygen,
        Flourine,
        Sodium,
        Magnesium,
        Chlorine,
        Phosphorus,
        Sulfur,
        Potasium,
        Calcium,
        Bromine
    };

    void setElement(Element changeElement);
    void setAtomPos(QPointF newPoint);
    void addBond(Bond *p_bond);
    void removeBond(Bond *p_bond);
    int getNumBonds();
    QVector<Bond*> bonds;
    QPointF atomPos;
    Element element;
};

#endif // ATOM_H
