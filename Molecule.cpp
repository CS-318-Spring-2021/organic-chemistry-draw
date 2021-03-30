#include "molecule.h"
#include <array>

Molecule::Molecule(QPointF drawnVertices[], int numVerts)
{
    QPointF origin = drawnVertices[0];
    setOrigin(origin);
    setBondLength(origin, drawnVertices[1]);
    Atom *p_firstAtom = new Atom(origin);
    setFirstAtom(p_firstAtom);
    Atom *p_previousAtom = firstAtom;

    for (int i=1; i < numVerts-1; i++) {
        Bond *p_currentBond = new Bond();
        p_previousAtom->setSecondBond(p_currentBond);
        p_currentBond->setFirstAtom(p_previousAtom);
        Atom *p_currentAtom = new Atom(drawnVertices[i]);
        p_currentAtom->setFirstBond(p_currentBond);
        p_currentBond->setSecondAtom(p_currentAtom);
        p_previousAtom= p_currentAtom;
    }
}


void Molecule::setOrigin(QPointF &firstPoint) {
    origin= firstPoint;
}

void Molecule::setBondLength(QPointF first, QPointF second) {
    bondLength = calculateDistance(first, second);
}

void Molecule::setFirstAtom(Atom *p_firstAtom) {
    firstAtom = p_firstAtom;
}

double Molecule::calculateDistance(QPointF first, QPointF second) {
    double xSquare = pow(second.x()- first.x(), 2);
    double ySquare = pow(second.y()- first.y(), 2);
    double distance = sqrt(xSquare+ySquare);
    return distance;
}
