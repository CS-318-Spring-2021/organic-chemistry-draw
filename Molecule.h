#ifndef MOLECULE_H
#define MOLECULE_H

#include "Atom.h"
#include "Bond.h"
#include <QPointF>
#include <QVector>

class Molecule{

    enum MoleculeType {Cyclic, Linear};

    double bondLength;
    MoleculeType type;
    QVector<Atom*> atomSet;
    QVector<Bond*> bondSet;


public:
    Molecule(QVector<QPointF> drawnVertices);
    void setBondLength(QPointF first, QPointF second);
    static double calculateDistance(QPointF first, QPointF second);
    QVector<Atom*> getAtomSet();
    QVector<Bond*> getBondSet();

};

#endif // MOLECULE_H
