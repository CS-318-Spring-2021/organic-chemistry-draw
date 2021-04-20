#include "Molecule.h"
#include <array>

Molecule::Molecule(QVector<QPointF> drawnVertices) {
    setBondLength(drawnVertices[0], drawnVertices[1]);
    Atom *p_currentAtom = new Atom(drawnVertices[0]);
    atomSet.append(p_currentAtom);
    Atom *p_previousAtom = p_currentAtom;

    for (int i=1; i <= drawnVertices.size(); i++) { //TODO: FIX THIS !! IT IS WACKADOODLE CRAZY TOWN

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

    if(drawnVertices[0].x() == drawnVertices[drawnVertices.size()-1].x()
             && drawnVertices[0].y() == drawnVertices[drawnVertices.size()-1].y()){
        type = Cyclic;
        correctCyclicStructure();
        //printf("%i\n", 0);
    } else{
        type = Linear;
        correctLineStructure();
    }

}


void Molecule::setBondLength(QPointF first, QPointF second) {
    bondLength = calculateDistance(first, second);
}

double Molecule::calculateDistance(QPointF first, QPointF second) {
    double xSquare = pow((second.x()- first.x()), 2);
    double ySquare = pow((second.y()- first.y()), 2);
    double distance = sqrt(xSquare+ySquare);
    return distance;
}

QVector<Atom*> Molecule::getAtomSet(){
    return atomSet;
}
QVector<Bond*> Molecule::getBondSet(){
    return bondSet;
}

void Molecule::correctLineStructure(){
    double y = sin(0.6196)*bondLength;
    double x = sin(0.9512)*bondLength;
    if (atomSet[1]->getAtomPos().y() - atomSet[0]->getAtomPos().y() < 0){
        y = y*(-1);
    }
    for(int i = 1; i<atomSet.size(); i++){
        double newXPos = atomSet[i-1]->getAtomPos().x() + x;
        double newYPos = atomSet[i-1]->getAtomPos().y() + y;
        y = y*(-1);
        atomSet[i]->setAtomPos(QPointF(newXPos,newYPos));
    }
}

void Molecule::correctCyclicStructure() {
    if (atomSet.size() == 7) {
        printf("%f\n", (atomSet[5]->getAtomPos().y() - atomSet[0]->getAtomPos().y())/bondLength);
        if ((atomSet[5]->getAtomPos().y() - atomSet[0]->getAtomPos().y())/bondLength > 0.2){
            // this is for a pointy top hexagon
            double x = sin(0.866)*bondLength;
            double y = sin(0.5)*bondLength;
            double x1 = atomSet[0]->getAtomPos().x() - x;
            double y1 = atomSet[0]->getAtomPos().y() + y;
            double x2 = x1;
            double y2 = y1 + bondLength;
            double x3 = atomSet[0]->getAtomPos().x();
            double y3 = y2 + y;
            double x4 = x2 + 2*x;
            double y4 = y2;
            double x5 = x4;
            double y5 = y1;
            // i think we can do or loop and use i as in xi;
            atomSet[1]->setAtomPos(QPointF(x1,y1));
            atomSet[2]->setAtomPos(QPointF(x2,y2));
            atomSet[3]->setAtomPos(QPointF(x3,y3));
            atomSet[4]->setAtomPos(QPointF(x4,y4));
            atomSet[5]->setAtomPos(QPointF(x5,y5));
        } else{
            //this is for a flat top hexagon
            double x = sin(0.5)*bondLength;
            double y = sin(0.866)*bondLength;
            double x1 = atomSet[0]->getAtomPos().x() - x;
            double y1 = atomSet[0]->getAtomPos().y() + y;
            double x2 = atomSet[0]->getAtomPos().x();
            double y2 = atomSet[0]->getAtomPos().y() + 2*y;
            double x3 = x2 + bondLength;
            double y3 = y2;
            double x4 = x3 + x;
            double y4 = y1;
            double x5 = x3;
            double y5 = atomSet[0]->getAtomPos().y();
            // i think we can do or loop and use i as in xi;
            atomSet[1]->setAtomPos(QPointF(x1,y1));
            atomSet[2]->setAtomPos(QPointF(x2,y2));
            atomSet[3]->setAtomPos(QPointF(x3,y3));
            atomSet[4]->setAtomPos(QPointF(x4,y4));
            atomSet[5]->setAtomPos(QPointF(x5,y5));
        }
    }
}
