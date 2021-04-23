#include "qbond.h"
#include "Bond.h"
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QBrush>

QBond::QBond(Bond *_bond):bond(_bond) {
    QPointF a = bond->atomFirst->atomPos;
    QPointF b = bond->atomSecond->atomPos;

    int quantity = bond->quantity;
    int quality = bond->quality;

    QLineF line(a, b);

    qgline = new QGraphicsLineItem(line);


    hoverCircle = new QGraphicsEllipseItem(line.center().x()-15, line.center().y()-15, 30, 30);
    hoverCircle->setPen(QPen(Qt::NoPen));
    hoverCircle->setBrush(QColor(0, 0, 0, 30));
    hoverCircle->setOpacity(0.0);

    addToGroup(hoverCircle);


    setAcceptHoverEvents(true);



    if (quality==0 && quantity==0){
        addToGroup(qgline);
    }
    else if (quantity>0){

        double LENGTHMODIFIER = 0.018;
        if (quantity==2){
            LENGTHMODIFIER = 0.03;
        }

        QLineF supplementary(a, b);
        supplementary.setLength(LENGTHMODIFIER*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerone = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornertwo = supplementary.p2();

        supplementary = QLineF(b, a);
        supplementary.setLength(LENGTHMODIFIER*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerthree = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornerfour = supplementary.p2();

        if (quantity==2){
            addToGroup(new QGraphicsLineItem(a.x(), a.y(), b.x(), b.y()));
        }
        addToGroup(new QGraphicsLineItem(cornerone.x(), cornerone.y(), cornerthree.x(), cornerthree.y()));
        addToGroup(new QGraphicsLineItem(cornertwo.x(), cornertwo.y(), cornerfour.x(), cornerfour.y()));


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

void QBond::hoverEnterEvent(QGraphicsSceneHoverEvent *evt) {
    hoverCircle->setOpacity(0.25);
}

void QBond::hoverLeaveEvent(QGraphicsSceneHoverEvent *evt) {
    hoverCircle->setOpacity(0.0);
}
