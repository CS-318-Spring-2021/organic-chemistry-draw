#include "qbond.h"
#include "Bond.h"
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QBrush>

QBond::QBond(Bond *_bond):bond(_bond)
{
    qgline = new QGraphicsLineItem(QLineF(bond->atomFirst->atomPos, bond->atomSecond->atomPos));

    QPointF a = bond->atomFirst->atomPos;
    QPointF b = bond->atomSecond->atomPos;

    int quantity = bond->quantity;
    int quality = bond->quality;

    if (quality==0 && quantity==0){
        addToGroup(qgline);
    }
    else if (quantity>0){

        double LENGTHMODIFIER = 0.018;
        if (quantity==2){
            LENGTHMODIFIER = 0.03;
        }
        QLineF line(a, b);
        QLineF supplementary(a, b);
        supplementary.setLength(LENGTHMODIFIER*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerone = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornertwo = supplementary.p2();

        supplementary = QLineF(a, b);
        supplementary.setLength(LENGTHMODIFIER*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerthree = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornerfour = supplementary.p2();

        if (quantity==2){
            addToGroup(new QGraphicsLineItem(line));
        }
        addToGroup(new QGraphicsLineItem(QLineF(cornerone, cornerthree)));
        addToGroup(new QGraphicsLineItem(QLineF(cornertwo, cornerfour)));
    }
    else if (quality>0){
        //xy coordinates for the two endpoints of the bond
        const double TRIANGLE = 0.05;
        QLineF line(a, b);
        QPointF perp(line.dy(), -line.dx());

        if (quality==1 || quality==2) {
            const int NBLACK = (quality==1 ? 1 : 8);
            const double WHITE = 1.25, BLACK = 1.0;  // Relative lengths of white and black
            double step = 1.0/(NBLACK*BLACK + (NBLACK-1)*WHITE);
            double t = 0.0; // 0.0 is a, 1.0 is b
            for (int i = 0; i < 2*NBLACK-1; ++i)
                if (i & 1) { // WHITE
                   t += WHITE*step;
                } else {
                    QPolygonF poly;
                    poly << line.pointAt(t) + (t+0.1)*TRIANGLE*perp << line.pointAt(t) - (t+0.1)*TRIANGLE*perp;
                    t += BLACK*step;
                    poly << line.pointAt(t) - (t+0.1)*TRIANGLE*perp << line.pointAt(t) + (t+0.1)*TRIANGLE*perp;
                    QGraphicsPolygonItem *item = new QGraphicsPolygonItem(poly);
                    item->setPen(Qt::NoPen);
                    item->setBrush(Qt::black);
                    addToGroup(item);
                }
        }
    }
}
