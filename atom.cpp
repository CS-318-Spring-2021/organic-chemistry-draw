#include "atom.h"

atom::atom(QPoint lastAtom){
    // set pendown (type) coordinates to atomPos
}

atom::atom(QPoint currentAtom, QPointF nextAtom) {
    setAtomPos(currentAtom);
    // set pendown (type) coordinates to atomPos
}

void setAtomPos(QPointF &pos) {
    atomPos = pos;
}
void addBond(QPointF vertex1, QPointF vertex2) {
    numBonds++;
}
