#ifndef MOLECULE_H
#define MOLECULE_H

#include <drawspace.h>
#include <atom.h>
#include <bond.h>

class molecule: public drawspace{

    enum MoleculeType {Cyclic, Linear};

    double bondLength;
    QPointF origin;
    MoleculeType type;
    atom *firstAtom;

public:
    molecule(QPointF drawnVertices[], int numVerts);

    void setOrigin(QPointF &firstPoint);
    void setBondLength(QPointF first, QPointF second);
    void setFirstAtom(atom *p_firstAtom);
    static double calculateDistance(QPointF first, QPointF second);


};

#endif // MOLECULE_H
