#ifndef QATOM_H
#define QATOM_H

#include <QGraphicsItemGroup>

class Bond;
class QBond : public QGraphicsItemGroup
{
public:
    QBond(Bond *_bond);
    Bond *bond;

};

#endif // QBOND_H
