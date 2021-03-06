
#ifndef MOLECULE_H
#define MOLECULE_H

#include "Atom.h"
#include "Bond.h"
#include <QPointF>
#include <QVector>
#include <cmath>

class Molecule{

public:
    Molecule(QVector<QPointF> drawnVertices, float bondLength = -1);
    //Molecule(const Molecule &originalMolecule);
    //This can be used in future to allow us to make deep copies of all items store in a Molecule
    double bondLength;
    QVector<Atom*> atomSet;
    QVector<Bond*> bondSet;

    void setBondLength(QPointF first, QPointF second);
    void setBondLength(float length);
    QVector<Atom*> correctStructure(QVector<Atom*> atoms, Atom * appendee, int nSides = 6);
    void addNewVerts(QVector<QPointF> drawnVertices);
    void addBond(Atom *p_start, Atom *p_finish);
    void removeAtom(Atom *p_atom);
    void removeBond(Bond *p_bond);
    void combine(Molecule *other, Atom *connecting);
};

#endif // MOLECULE_H
