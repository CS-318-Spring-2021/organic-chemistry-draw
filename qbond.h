#ifndef QBOND_H
#define QBOND_H

#include <QGraphicsItemGroup>

class Bond;
class QBond : public QGraphicsItemGroup
{
public:
    QBond(Bond *_bond);
    Bond *bond;
    QGraphicsLineItem *qgline;

};

#endif // QBOND_H
