#ifndef MOLECULE_H
#define MOLECULE_H

#include <drawspace.h>
#include "Atom.h"
#include "Bond.h"

class Molecule: public drawspace{

    enum MoleculeType {Cyclic, Linear};

    double bondLength;
    QPointF origin;
    MoleculeType type;
    Atom *firstAtom;

public:
    Molecule(QPointF *drawnVertices, int numVerts);

    void setOrigin(QPointF &firstPoint);
    void setBondLength(QPointF first, QPointF second);
    void setFirstAtom(Atom *p_firstAtom);
    static double calculateDistance(QPointF first, QPointF second);


};

#endif // MOLECULE_H
