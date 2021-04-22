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
        QLineF line(b, a);
        QLineF supplementary(b, a);
        supplementary.setLength(TRIANGLE*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerone = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornertwo = supplementary.p2();
        addToGroup(new QGraphicsLineItem(QLineF(cornerone, cornertwo)));

        QVector<QPointF> points;
        points.append(a);
        points.append(cornerone);
        points.append(cornertwo);

        if (quality==1){
            addToGroup(new QGraphicsPolygonItem(QPolygonF(points)));
            //QPen(Qt::black, 2.0)
            //QBrush(Qt::SolidPattern)
        }
        if (quality==2){
            QBrush br(Qt::VerPattern);
            QTransform t;
            t = t.inverted();
            t.rotate(0-line.angle()).scale(1,1);
            br.setTransform(t);
            addToGroup(new QGraphicsPolygonItem(QPolygonF(points)));
            //QPen(Qt::white, 0)
            //brush br
        }
    }
}
