#include "Molecule.h"
#include <array>
#include <QLineF>
#include <QDebug>

Molecule::Molecule(QVector<QPointF> drawnVertices) { //TODO: addBond should be a part of THIS class. adds an edge between two vertices. sets their neighbors, updates edgeset.
    setBondLength(drawnVertices[0], drawnVertices[1]); //ok we have a set bond length as the first value? not the average?
/*=======
Molecule::Molecule(QVector<QPointF> drawnVertices) {
    setBondLength(drawnVertices[0], drawnVertices[1]);
    Atom *p_currentAtom = new Atom(drawnVertices[0]);
    atomSet.append(p_currentAtom);
    Atom *p_previousAtom = p_currentAtom;
//    printf("%i\n", drawnVertices.size());
>>>>>>> 2d43ca08ee7d03ceb60621edde6009c19acf6b0e*/

    Atom *p_currentAtom = new Atom(drawnVertices[0]);   //current atom
    atomSet.append(p_currentAtom);                      //add to atomSet
    Atom *p_previousAtom = p_currentAtom;               //previous atom = current

    for (int i=1; i< drawnVertices.size(); i++){                            //for the rest of the vertices in drawnVertices--

        Atom *p_currentAtom = new Atom(drawnVertices[i]);                   //make a new atom out of the vertex
        Bond *p_currentBond = new Bond(p_currentAtom, p_previousAtom);      //create edge btw old and new

        p_currentAtom->addBond(p_currentBond);                              //give new vertex this bond
        p_previousAtom->addBond(p_currentBond);                             //give old vertex this bond

        atomSet.append(p_currentAtom);                                      //add vertex to graph
        bondSet.append(p_currentBond);                                      //add edge to graph

        p_previousAtom = p_currentAtom;                                     //reset this value for the next iteration

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

void Molecule:: addNewVerts(QVector<QPointF> drawnVertices){
    //when do you make double v triple? great question. i will not answer it B)

                                                                            //the first item in drawnVertices is an existing atom.
    QPointF finding = drawnVertices[0];                                     //so we loop through existing atoms.
    Atom *p_currentAtom = atomSet[0];                                      //
    int smallest = calculateDistance(finding, p_currentAtom->getAtomPos());// find the one that's closest to dV[0]
    for (int i=1; i<atomSet.size(); i++){                                   //
        int d = calculateDistance(finding, atomSet[i]->getAtomPos());       // and remember it
        if (d < smallest){                                                  //
            smallest = d;                                                   //
            p_currentAtom = atomSet[i];                                    // (remember it as p_previousAtom)
        }
    }

    Atom *p_previousAtom = p_currentAtom;

    for (int i=1; i< drawnVertices.size(); i++){                            //for the REST of the vertices in drawnVertices--

        Atom *p_currentAtom = new Atom(drawnVertices[i]);                   //make a new atom out of the vertex
        Bond *p_currentBond = new Bond(p_currentAtom, p_previousAtom);      //create edge btw old and new-- shouldn't this be in molecule class?

        p_currentAtom->addBond(p_currentBond);                              //give new vertex this bond
        p_previousAtom->addBond(p_currentBond);                             //give old vertex this bond

        atomSet.append(p_currentAtom);                                      //add vertex to graph
        bondSet.append(p_currentBond);                                      //add edge to graph

        p_previousAtom = p_currentAtom;                                     //reset this value for the next iteration

    }
    /*

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
    }*/
}
