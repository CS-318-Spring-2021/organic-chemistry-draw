#include "qbond.h"
#include "Bond.h"
#include "mainwindow.h"
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QBrush>

QBond::QBond(Bond *_bond):bond(_bond) {
    QPointF p1 = bond->atomFirst->atomPos;
    QPointF p2 = bond->atomSecond->atomPos;

    int quantity = bond->quantity;
    int drawStyle = bond->drawStyle;

    QLineF line(p1, p2);

    qgline = new QGraphicsLineItem(line);

    hoverCircle = new QGraphicsEllipseItem(line.center().x()-15, line.center().y()-15, 30, 30); //needs to use trig to adjust the TL of the circle
    hoverCircle->setPen(QPen(Qt::NoPen));
    hoverCircle->setBrush(QColor(0, 0, 0, 30));
    hoverCircle->setOpacity(0.0);

    addToGroup(hoverCircle);

    setAcceptHoverEvents(true);

    if (drawStyle==0 || quantity>1){ //anything other than weighted and dashed
        double WIDTHMODIFIER = .02;
        double LENGTHMODIFIER = 0.03;
        if (quantity==3) LENGTHMODIFIER = 0.04;
        QLineF supplementary(p1, p2);
        supplementary.setLength(LENGTHMODIFIER*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerone = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornertwo = supplementary.p2();

        supplementary = QLineF(p2, p1);
        supplementary.setLength(LENGTHMODIFIER*line.length());
        supplementary.setAngle(line.angle()+90);
        QPointF cornerthree = supplementary.p2();
        supplementary.setAngle(line.angle()-90);
        QPointF cornerfour = supplementary.p2();

        QGraphicsLineItem* leftline = new QGraphicsLineItem(cornerone.x(), cornerone.y(), cornerthree.x(), cornerthree.y());
        QGraphicsLineItem* centerline = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y());
        QGraphicsLineItem* rightline = new QGraphicsLineItem(cornertwo.x(), cornertwo.y(), cornerfour.x(), cornerfour.y());
        leftline->setPen(QPen(Qt::black, WIDTHMODIFIER*line.length()));
        centerline->setPen(QPen(Qt::black, WIDTHMODIFIER*line.length())); 
        rightline->setPen(QPen(Qt::black, WIDTHMODIFIER*line.length()));
        if (quantity==1){
            addToGroup(centerline);
            return;
        }
        addToGroup(leftline);
        addToGroup(rightline);
        if (quantity==3) addToGroup(centerline);
    }
    else if (quantity==1){ //weighted and dashed
        //xy coordinates for the two endpoints of the bond
        const double TRIANGLE = 0.05;
        QLineF line(p1, p2);
        QPointF perp(line.dy(), -line.dx());

        if (drawStyle==1 || drawStyle==2) {
            const int NBLACK = (drawStyle==1 ? 1 : 8);
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

void QBond::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hoverCircle->setOpacity(0.5);
}

void QBond::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    hoverCircle->setOpacity(0.0);
}

void QBond::mousePressEvent(QGraphicsSceneMouseEvent *) {
    //if undo is clicked
        //delete?
    //else
    bond->changeBond();
}
