#ifndef MOLECULE_H
#define MOLECULE_H

#include <drawspace.h>

class atom;

class molecule: public drawspace{

    double bondLength;
    QPointF origin;
    MoleculeType type;
    atom *firstAtom;
public:
    molecule(QPointF drawnVertices[], int numVerts);

    void setOrigin(QPointF &firstPoint);
    void setBondLength(QPointF first, QPointF second);
    void setFirstAtom(atom *p_firstAtom);
    double calculateDistance(QPointF first, QPointF second);
};

#endif // MOLECULE_H
