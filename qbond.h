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
    QGraphicsEllipseItem *hoverCircle;

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *evt);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt);
};

#endif // QBOND_H
