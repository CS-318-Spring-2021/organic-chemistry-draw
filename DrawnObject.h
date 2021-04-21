#ifndef DRAWNOBJECT_H
#define DRAWNOBJECT_H
#include <QWidget>

#include "drawspace.h"

class DrawnObject{
    drawspace* view;
public:
    QVector<QPointF*> positionInputPoints;
    QVector<int*> timeInputPoints;
    QVector<QPointF> vertices;

    DrawnObject(drawspace* view);
    void addData(QPointF position, int time);
    ~DrawnObject();
    void clean();
    QVector<QPointF> analyze();
    QVector<QPointF> analyzeLengths(QVector<QPointF> vertices);
    QVector<QPointF> eliminateColinear(QVector<QPointF> vertices);
    QPointF pointAverage(QVector<QPointF> points);
    int maxLength(QVector<QPointF> points);

};

#endif // DRAWNOBJECT_H
