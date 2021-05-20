
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
    enum MoleculeType {Cyclic, Linear};
    double bondLength;
    MoleculeType type;
    QVector<Atom*> atomSet;
    QVector<Bond*> bondSet;
    const double standardLineSegmentAngle = (35.25*M_PI)/180.0;
    void setBondLength(QPointF first, QPointF second);
    void setBondLength(float length);
    QVector<Atom*> correctStructure(QVector<Atom*> atoms, Atom * appendee, int nSides = 6);
    void addNewVerts(QVector<QPointF> drawnVertices);
    void addBond(Atom *p_start, Atom *p_finish);
    void removeAtom(Atom *p_atom);
    void removeBond(Bond *p_bond);
    void combine(Molecule *other, Atom *connecting);
    double upFirstCorrect(double y2, double y3, double theta);
    double downFirstCorrect(double y2, double y3, double theta);
};

#endif // MOLECULE_H
