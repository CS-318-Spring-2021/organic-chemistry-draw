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

//    void setFirstBond(Bond *p_firstBond);
//    void setSecondBond(Bond *p_secondBond);
//    void setThirdBond(Bond *p_thirdBond);
//    void setFourthBond(Bond *p_fourthBond);
    void setElement(Element changeElement);
    void setAtomPos(QPointF newPoint);
    QPointF getAtomPos();
    void addBond(Bond *p_bond);
    QPointF getPos();

private:
    QVector<Bond*> bonds;
    //int numBonds = 0;
    //Bond *bondFirst;
    //Bond *bondSecond;
    //Bond *bondThird;
    //Bond *bondFourth;
    QPointF atomPos;
    double const atomRadius = 10; //need to decide this later
    Element element;



};

#endif // ATOM_H
