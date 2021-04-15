#include "Molecule.h"
#include <array>
#include <QLineF>
#include <QDebug>

Molecule::Molecule(QVector<QPointF> drawnVertices) {
    setBondLength(drawnVertices[0], drawnVertices[1]);
    Atom *p_currentAtom = new Atom(drawnVertices[0]);
    atomSet.append(p_currentAtom);
    Atom *p_previousAtom = p_currentAtom;
//    printf("%i\n", drawnVertices.size());

    for (int i=1; i < drawnVertices.size(); i++) {

//        printf("%i:(%i, %i)\n", i, int((drawnVertices[i]).x()), int((drawnVertices[i]).y()));
//        printf("%i\n",int(bondLength));

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
    return QLineF(first,second).length();
}

QVector<Atom*> Molecule::getAtomSet(){
    return atomSet;
}
QVector<Bond*> Molecule::getBondSet(){
    return bondSet;
}

void Molecule::correctLineStructure(){
    double y = sin(standardLineSegmentAngle)*bondLength;
    double x = cos(standardLineSegmentAngle)*bondLength;
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

    int numVerts = atomSet.size()-1;
    assert(numVerts >1);
    QPointF center(0,0);
    for(int i = 0; i < numVerts; i++){
        center += atomSet[i]->getAtomPos();
    }
    center/=numVerts;

    QVector<double> angles(numVerts);

    for(int i = 0; i < numVerts; i++){
        angles[i] = QLineF(center, atomSet[i]->getAtomPos()).angle();
    }

    double firstAngle = angles[0];

    firstAngle = round(firstAngle/(180/numVerts))*(M_PI/numVerts);

    double radius = bondLength/(2*sin(M_PI/numVerts));

    for(int i = 0; i <= numVerts; i++){
        double theta = firstAngle + i*((2*M_PI)/numVerts);
        atomSet[i]->setAtomPos(center+radius*QPointF(cos(theta),sin(theta)));
    }
}
