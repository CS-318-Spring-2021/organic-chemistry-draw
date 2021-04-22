#ifndef ATOM_H
#define ATOM_H

#include "Molecule.h"
#include <QPointF>
#include <QVector>

class Bond;

class Atom{
public:
    Atom(QPointF currentPos);

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
    QPointF getAtomPos();
    void addBond(Bond *p_bond);
    QPointF getPos();
    QVector<Bond*> bonds;
    //int numBonds = 0;
    //Bond *bondFirst;
    //Bond *bondSecond;
    //Bond *bondThird;
    //Bond *bondFourth;
    QPointF atomPos;
    double const atomRadius = 10; //need to decide this later
    Element element;
    void printAtom();
    void printAtomDetailed();



};

#endif // ATOM_H
