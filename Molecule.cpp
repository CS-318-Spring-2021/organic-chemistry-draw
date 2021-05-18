#include "Molecule.h"
#include <array>
#include <QLineF>
#include <QDebug>

Molecule::Molecule(QVector<QPointF> drawnVertices) {
    //TODO:
    //if there is one line off of the hex or structure
    //if another one goes in, then change to wedges and dashes and reset the angles
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
        //correctCyclicStructure();
        correctLineStructure(atomSet,nullptr,drawnVertices.size()-1);
    } else{
        type = Linear;
        atomSet = correctLineStructure(atomSet, nullptr);
    }

}

Molecule::Molecule(const Molecule &originalMolecule) {
    //to be used to deepCopy the molecules to be used in undo
}


void Molecule::setBondLength(QPointF first, QPointF second) { //set length
    bondLength = QLineF(first, second).length();
}

QVector<Atom*> Molecule::correctLineStructure(QVector<Atom*> atoms, Atom * appendee, int nSides){
    if(appendee){
        Atom * preAppendee;
        if(appendee->bonds[0]->atomFirst==appendee){
            preAppendee = appendee->bonds[0]->atomSecond;
        }else preAppendee = appendee->bonds[0]->atomFirst;
        atoms.push_front(appendee);
        atoms.push_front(preAppendee);
        //push front atom before appendee
    }
    if(atoms.size()<3) return atoms;
    QPointF point1 = atoms[0]->atomPos;
    QPointF point2 = atoms[1]->atomPos;
    QPointF point3 = atoms[2]->atomPos;
    QLineF previousLine(point1, point2);
    double length = previousLine.length();
    //compute the angles first and use those
    for(int i = 2; i<atoms.size();i++){
        point2 = atoms[i-1]->atomPos;
        point3 = atoms[i]->atomPos;
        QLineF nextLine(point2, point3);
        double theta = previousLine.angleTo(nextLine);
        theta =((theta>=180.0) ? -1:1)*(360/nSides);
        nextLine.setLength(length);
        nextLine.setAngle(theta + previousLine.angle());
        atoms[i]->setAtomPos(nextLine.p2());
        previousLine = nextLine;
    }
    return atoms;
}



void Molecule::correctCyclicStructure() {

    removeAtom(atomSet.last());
    //bondSet.last()->atomFirst->bonds.removeLast();
    //bondSet.removeLast();

    int numVerts = atomSet.size();
    assert(numVerts >1);
    QPointF center(0,0);
    for(int i = 0; i < numVerts; i++){
        center += atomSet[i]->atomPos;
    }
    center/=numVerts;

    QVector<double> angles(numVerts);


    for(int i = 0; i < atomSet.size(); i++){

        //printf("%i: (%i, %i)\n", i, int(atomSet[i]->atomPos.x()), int(atomSet[i]->atomPos.y()));
        angles[i] = QLineF(center, atomSet[i]->atomPos).angle();
    }

    double firstAngle = angles[0];

    firstAngle = round(firstAngle/(180/numVerts))*(M_PI/numVerts);

    double radius = bondLength/(2*sin(M_PI/numVerts));

    for(int i = 0; i < atomSet.size(); i++){ //why did this use <=? don't we want to get rid of the last atom? because it's a repeat
        double theta = firstAngle + i*((2*M_PI)/numVerts);
        atomSet[i]->setAtomPos(center+radius*QPointF(cos(theta),sin(theta)));
    }

    addBond(atomSet.last(), atomSet.first());
    /*printf("\n");

    for(int i = 0; i < bondSet.size(); i++){
        printf("btw ");
        printf("(%i, %i)", int(bondSet[i]->atomFirst->atomPos.x()), int(bondSet[i]->atomFirst->atomPos.y()));
        printf(" and ");
        printf("(%i, %i)", int(bondSet[i]->atomSecond->atomPos.x()), int(bondSet[i]->atomSecond->atomPos.y()));
        printf("\n");

    }*/

}

void Molecule:: addNewVerts(QVector<QPointF> drawnVertices){ //adds a set of points to the graph one after another
    //the first item in drawnVertices is an existing atom, not sure yet which one though--
    QPointF finding = drawnVertices[0];                                     //so we loop through existing atoms,
    Atom *p_currentAtom = atomSet[0];
    int smallest = QLineF(finding, p_currentAtom->atomPos).length(); //find the one that's closest to dV[0]
    for (int i=1; i<atomSet.size(); i++){
        int d = QLineF(finding, atomSet[i]->atomPos).length();       // and remember it
        if (d < smallest){
            smallest = d;
            p_currentAtom = atomSet[i];                                     //(remember it as p_previousAtom)
        }
    }
    Atom *p_smallestDistanceAtom = p_currentAtom;
    Atom *p_previousAtom = p_currentAtom;
    QVector<Atom*> atomsToBeCleaned;
    for (int i=1; i< drawnVertices.size(); i++){                    //now for each of the vertices in this set that we are adding, we:
        Atom *p_currentAtom = new Atom(drawnVertices[i]);                   //make a new atom out of the vertex
        addBond(p_previousAtom, p_currentAtom);                             //add a bond between the two to the molecule
        atomsToBeCleaned.append(p_currentAtom);                                      //add the atom to the set we are sending to the correctLineStructure
        p_previousAtom = p_currentAtom;                                     //move to next one
    }
    QVector<Atom*> cleanedAtoms = correctLineStructure(atomsToBeCleaned, p_smallestDistanceAtom);
    for(int i = 2; i<cleanedAtoms.size(); i++){
        atomSet.append(cleanedAtoms[i]);
    }
}

void Molecule:: addBond(Atom *p_start, Atom *p_finish){
    //when he draws in the THIRD bond it will be skewed off correctly if he anticipates drawing the fourth
    //          could want angles freehanded?
    QLineF line(p_start->atomPos, p_finish->atomPos);
    line.setLength(this->bondLength);
    //float angle = 0;
    QPointF average(0,0);
    if(p_start->getNumBonds()==2){
        average -= 2*(p_start->atomPos);
        average += p_start->bonds[0]->atomFirst->atomPos;
        average += p_start->bonds[0]->atomSecond->atomPos;
        average += p_start->bonds[1]->atomFirst->atomPos;
        average += p_start->bonds[1]->atomSecond->atomPos;
        average /= 2;
        line = QLineF(p_start->atomPos, average);
        line.setLength(-1*(this->bondLength));
    }
    p_finish->atomPos = line.p2();
    Bond *p_bond = new Bond(p_start, p_finish);
    p_start->addBond(p_bond);
    p_finish->addBond(p_bond);
    bondSet.append(p_bond);
}

void Molecule:: removeAtom(Atom *p_atom){
    for (int i=0; i<atomSet.size(); i++){
        if (atomSet[i] == p_atom){
            atomSet.remove(i);
            break;
        }
    }
    //remove all the bonds that are connected to it
    for (int i = 0; i < p_atom->bonds.size(); i++){
        //go through p_atom's bonds--
        removeBond(p_atom->bonds[i]);
        if (p_atom->bonds[i]->atomFirst==p_atom){ //if we're deleting atomFirst
            p_atom->bonds[i]->atomSecond->removeBond(p_atom->bonds[i]);
        } else if (p_atom->bonds[i]->atomSecond==p_atom){ //if we're deleting atomSecond
            p_atom->bonds[i]->atomFirst->removeBond(p_atom->bonds[i]);

        }
    }
    p_atom->bonds.clear();
}

void Molecule:: removeBond(Bond *p_bond){
    for (int i=0; i<bondSet.size(); i++){
        if (bondSet[i] == p_bond){
            bondSet.remove(i);
            return;
        }
    }
}

void Molecule:: combine(Molecule *other, Atom *connecting){
    return;
    //connecting is the atom in 'other' that is actually an atom in our molecule
    //so we want to find which atom connecting is supposed to be in the molecule object, and add the bonds to it that connecting had
    //then we want to add all atoms besides connecting (it already exists)
    //we also want to add all of the bonds that exist in other, excepting the ones that involve 'connecting' (those were already taken care of
}
