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
    QVector<QPointF> altVertices;

    DrawnObject(drawspace* view);
    void addData(QPointF position, int time);
    ~DrawnObject();
    void clean();
    QVector<QPointF> analyze();
    QVector<QPointF> analyzeLengths(QVector<QPointF> vertices);
    QVector<QPointF> eliminateColinear(QVector<QPointF> vertices);
    QPointF pointAverage(QVector<QPointF> points);
    QVector<QPointF> analyzeWithSlopes();
    int binarySearch(int start, int end, float slope, float tolerence);

};

#endif // DRAWNOBJECT_H
