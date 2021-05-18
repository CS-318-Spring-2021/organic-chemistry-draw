#ifndef DRAWNOBJECT_H
#define DRAWNOBJECT_H
#include <QWidget>

#include "drawspace.h"

class DrawnObject{
public:
    QVector<QPointF> positionInputPoints;
    QVector<int> timeInputPoints;
    QVector<QPointF> vertices;

    DrawnObject();
    void addData(QPointF position, int time);
    void addData(QPointF position);
    ~DrawnObject();
    void clean();
    void cleanFreehand();
    void analyzeSpeed();
    void analyzeDistances();
    void analyzeColinearity();
    QPointF pointAverage(QVector<QPointF> points);
    int maxLength(QVector<QPointF> points);

};

#endif // DRAWNOBJECT_H
