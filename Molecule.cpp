#include "Molecule.h"
#include <array>
#include <QLineF>
#include <QDebug>

Molecule::Molecule(QVector<QPointF> drawnVertices) { //initialize molecule with this constructor: we have a input set of vertices. make vertices and edges in a line, but in a cycle if it's connected at the end.
                    //TODO: addBond should be a part of THIS class. adds an edge between two vertices. sets their neighbors, updates edgeset.
    setBondLength(drawnVertices[0], drawnVertices[1]); //ok we have a set bond length as the first value? not the average?

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


void Molecule::setBondLength(QPointF first, QPointF second) { //set length
    bondLength = calculateDistance(first, second);
}

double Molecule::calculateDistance(QPointF first, QPointF second) { //calculate distance? this should not be in molecule.... TODO: this should exist in Qpoint somewhere already.
    return QLineF(first,second).length();
}

QVector<Atom*> Molecule::getAtomSet(){ //TODO: this is hopefully superfluous
    return atomSet;
}
QVector<Bond*> Molecule::getBondSet(){ //TODO: this is also hopefully superfluous
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

void Molecule:: addNewVerts(QVector<QPointF> drawnVertices){ //adds a set of points to the graph one after another
    //when do you make double v triple? great question. i will not answer it B)

    //the first item in drawnVertices is an existing atom, not sure yet which one though--
    QPointF finding = drawnVertices[0];                                     //so we loop through existing atoms,
    Atom *p_currentAtom = atomSet[0];
    int smallest = calculateDistance(finding, p_currentAtom->getAtomPos()); //find the one that's closest to dV[0]
    for (int i=1; i<atomSet.size(); i++){
        int d = calculateDistance(finding, atomSet[i]->getAtomPos());       // and remember it
        if (d < smallest){
            smallest = d;
            p_currentAtom = atomSet[i];                                     //(remember it as p_previousAtom)
        }
    }

    Atom *p_previousAtom = p_currentAtom;

    for (int i=1; i< drawnVertices.size(); i++){                    //now for each of the vertices in this set that we are adding, we:
        Atom *p_currentAtom = new Atom(drawnVertices[i]);                   //make a new atom out of the vertex
        addBond(p_previousAtom, p_currentAtom);                             //add a bond between the two to the molecule
        atomSet.append(p_currentAtom);                                      //add the atom to the molecule
        p_previousAtom = p_currentAtom;                                     //move to next one
    }
}

void Molecule:: addBond(Atom *p_start, Atom *p_finish){
    Bond *p_bond = new Bond(p_start, p_finish); //this is the point where i SHOULD be able to recognize how many neighbors this atom has, right?
    p_start->addBond(p_bond);
    p_finish->addBond(p_bond);
    bondSet.append(p_bond);


}
