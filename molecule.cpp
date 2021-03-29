#include "molecule.h"
#include <array>

molecule::molecule(QPointF drawnVertices[], int numVerts)
{
    // need them to give us the size of array
    setOrigin(drawnVertices[0]);
    setBondLength(drawnVertices[0], drawnVertices[1]);
    atom *p_firstAtom = new atom(drawnVertices[0]);
    setFirstAtom(p_firstAtom);
    atom *p_previousAtom = firstAtom;

    for (int i=1; i < numVerts-1; i++) {
        bond *p_currentBond = new bond();
        p_previousAtom->setSecondBond(p_currentBond);
        p_currentBond->setFirstAtom(p_previousAtom);
        atom *p_currentAtom = new atom(drawnVertices[i]);
        p_currentAtom->setFirstBond(p_currentBond);
        p_currentBond->setSecondAtom(p_currentAtom);
        p_previousAtom= p_currentAtom;
    }
}


void molecule::setOrigin(QPointF &firstPoint) {
    origin= firstPoint;
}

void molecule::setBondLength(QPointF first, QPointF second) {
    bondLength = calculateDistance(first, second);;
}

void molecule::setFirstAtom(atom *p_firstAtom) {
    firstAtom = p_firstAtom;
}

static double calculateDistance(QPointF first, QPointF second) {
    double xSquare = pow(second.x()- first.x(), 2);
    double ySquare = pow(second.y()- first.y(), 2);
    double distance = sqrt(xSquare+ySquare);
    return distance;
}
