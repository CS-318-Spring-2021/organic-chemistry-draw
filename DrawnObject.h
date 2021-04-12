#ifndef DRAWNOBJECT_H
#define DRAWNOBJECT_H
#include <QWidget>

#include "drawspace.h"

class DrawnObject{
    //QVector<int*> vector;
    drawspace* view;
public:
    QVector<int*> vector;
    QVector<QPointF> vertices;
    //QVector<int> indices;
    //QVector<int*> vertices2;
    //QVector<float> speeds;
    //QVector<QPointF> cleanedVertices;
    DrawnObject(drawspace* view);
    void addData(int x, int y, int time);
    ~DrawnObject();
    void clean();
    QVector<QPointF> analyze();
    QVector<QPointF> analyzeLengths(QVector<QPointF> vertices);
    //float speedCalc(int index);
    //void analyzeWithSlopes(int gap);
    //int binarySearch(int start, int end, float slope, float tolerence);
    void drawVerticesx(QVector<QPointF> vertices, QPen pen);
    void drawVerticesy(QVector<QPointF> vertices, QPen pen);
    QVector<QPointF> eliminateColinear(QVector<QPointF> vertices);
    QPointF pointAverage(QVector<QPointF> points);

};

#endif // DRAWNOBJECT_H
