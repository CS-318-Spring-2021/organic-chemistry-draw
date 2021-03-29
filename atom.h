#ifndef ATOM_H
#define ATOM_H

#include <molecule.h>
#include <drawspace.h>

class bond;

class atom{
public:
    atom(QPointF currentPos);

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

    void setFirstBond(bond *p_firstBond);
    void setSecondBond(bond *p_secondBond);
    void setThirdBond(bond *p_thirdBond);
    void setFourthBond(bond *p_fourthBond);
    void setElement(Element changeElement);

private:
    int numBonds = 0;
    bond *bondFirst;
    bond *bondSecond;
    bond *bondThird;
    bond *bondFourth;
    QPointF atomPos;
    double const atomRadius = 10; //need to decide this later
    Element element;



};

#endif // ATOM_H
