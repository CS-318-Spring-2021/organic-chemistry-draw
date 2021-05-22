#include "Molecule.h"
#include <array>
#include <QLineF>
#include <QDebug>

Molecule::Molecule(QVector<QPointF> drawnVertices, float bondLength) {
    if(bondLength == -1){
        setBondLength(drawnVertices[0], drawnVertices[1]);
    }else{
        setBondLength(bondLength);
    }

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
        correctStructure(atomSet,nullptr,drawnVertices.size()-1);
    } else{
        type = Linear;
        atomSet = correctStructure(atomSet, nullptr);
    }

}

//Molecule::Molecule(const Molecule &originalMolecule) {
    //to be used to deepCopy the molecules to be used in undo
//}


void Molecule::setBondLength(QPointF first, QPointF second) { //set length
    bondLength = QLineF(first, second).length();
}

void Molecule::setBondLength(float length) { //set length
    bondLength = length;
}

QVector<Atom*> Molecule::correctStructure(QVector<Atom*> atoms, Atom * appendee, int nSides){
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
    QLineF savePreviousLine(point1, point2);
    double length = bondLength;
    QVector<bool> upDownArray;
    upDownArray.append(false);
    upDownArray.append(false);
    //appends two bools so we are even with the atoms.size()
    //calculates angles before we start changing the position of the atoms
    for(int i = 2; i<atoms.size();i++){
        point2 = atoms[i-1]->atomPos;
        point3 = atoms[i]->atomPos;
        QLineF nextLine(point2, point3);
        bool upDown =((previousLine.angleTo(nextLine)>=180.0) ? true:false);
        upDownArray.append(upDown);
        previousLine = nextLine;
    }
    previousLine = savePreviousLine;
    previousLine.setLength(bondLength);
    atoms[1]->setAtomPos(previousLine.p2());
    for(int i = 2; i<atoms.size();i++){
        point2 = atoms[i-1]->atomPos;
        point3 = atoms[i]->atomPos;
        QLineF nextLine(point2, point3);
        double theta =((upDownArray[i]) ? -1:1)*(360/nSides);
        nextLine.setLength(length);
        nextLine.setAngle(theta + previousLine.angle());
        atoms[i]->setAtomPos(nextLine.p2());
        previousLine = nextLine;
    }
    return atoms;
}

void Molecule:: addNewVerts(QVector<QPointF> drawnVertices){ //adds a set of points to the graph one after another
    //the first item in drawnVertices is an existing atom
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
    QVector<Atom*> cleanedAtoms;
    if(p_smallestDistanceAtom->bonds.size()>3){
        Atom * preAppendee;
        if(p_smallestDistanceAtom->bonds[2]->atomFirst==p_smallestDistanceAtom){
            preAppendee = p_smallestDistanceAtom->bonds[2]->atomSecond;
        }else preAppendee = p_smallestDistanceAtom->bonds[2]->atomFirst;
        if(p_smallestDistanceAtom->bonds[3]->atomFirst==p_smallestDistanceAtom){
            p_currentAtom = p_smallestDistanceAtom->bonds[3]->atomSecond;
        }else p_currentAtom = p_smallestDistanceAtom->bonds[3]->atomFirst;
        //at this point, preAppendee is the first point on the third line off this atom,
        //and p_currentAtom is the first point on the fourth line off this atom.
        QLineF previousLine(p_smallestDistanceAtom->atomPos,preAppendee->atomPos);
        QLineF nextLine(p_smallestDistanceAtom->atomPos, p_currentAtom->atomPos);
        double theta =previousLine.angleTo(nextLine);
        if(theta>180) theta = 360 - theta;
        nextLine.setLength(this->bondLength);
        atomsToBeCleaned.pop_front();
        cleanedAtoms = correctStructure(atomsToBeCleaned, p_currentAtom);
        //corrects the rest of the fourth line off the atom
        for(int i = 0; i<cleanedAtoms.size(); i++){
            if(!atomSet.contains(cleanedAtoms[i]))atomSet.append(cleanedAtoms[i]);
        }
        if(previousLine.angle()<nextLine.angle()){ //if we draw to the right of the vertical or the left
            previousLine.setAngle(previousLine.angle()-theta);
        }else{
            previousLine.setAngle(previousLine.angle()+theta);
        }
        atomsToBeCleaned.clear();
        //remakes atomsToBeCleaned to correct the rest of the third line off the atom
        if(preAppendee->bonds.size()>1){
            Atom * atom = preAppendee;
            while(atom->bonds.size()>1){
                atom = atom->bonds[1]->atomSecond;
                atomsToBeCleaned.append(atom);
            }
        }
        preAppendee->atomPos = previousLine.p2();
        cleanedAtoms = correctStructure(atomsToBeCleaned, preAppendee);
    }else{
        cleanedAtoms = correctStructure(atomsToBeCleaned, p_smallestDistanceAtom);
    }
    for(int i = 0; i<cleanedAtoms.size(); i++){
        if(!atomSet.contains(cleanedAtoms[i]))atomSet.append(cleanedAtoms[i]);
    }
}

void Molecule:: addBond(Atom *p_start, Atom *p_finish){
    //when he draws in the THIRD bond it will be skewed off correctly if he anticipates drawing the fourth
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
