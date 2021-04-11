#include "Molecule.h"
#include <array>

Molecule::Molecule(QVector<QPointF> drawnVertices) {
    setBondLength(drawnVertices[0], drawnVertices[1]); //TODO: this isnt always right
    Atom *p_currentAtom = new Atom(drawnVertices[0]);
    atomSet.append(p_currentAtom);
    Atom *p_previousAtom = p_currentAtom;
    for (int i=1; i < drawnVertices.size(); i++) {

        printf("%i:(%i, %i)\n", i, int((drawnVertices[i]).x()), int((drawnVertices[i]).y()));

        Bond *p_currentBond = new Bond();
        p_previousAtom->addBond(p_currentBond);
        p_currentBond->setFirstAtom(p_previousAtom);
        Atom *p_currentAtom = new Atom(drawnVertices[i]);
        p_currentAtom->addBond(p_currentBond);
        p_currentBond->setSecondAtom(p_currentAtom);
        bondSet.append(p_currentBond);
        p_previousAtom= p_currentAtom;
        atomSet.append(p_currentAtom);
    }
}


void Molecule::setBondLength(QPointF first, QPointF second) {
    bondLength = calculateDistance(first, second);
}

double Molecule::calculateDistance(QPointF first, QPointF second) {
    double xSquare = pow(second.x()- first.x(), 2);
    double ySquare = pow(second.y()- first.y(), 2);
    double distance = sqrt(xSquare+ySquare);
    return distance;
}

QVector<Atom*> Molecule::getAtomSet(){
    return atomSet;
}
QVector<Bond*> Molecule::getBondSet(){
    return bondSet;
}
