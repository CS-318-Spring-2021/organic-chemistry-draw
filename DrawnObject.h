#ifndef DRAWNOBJECT_H
#define DRAWNOBJECT_H
#include <QWidget>

#include "drawspace.h"

class DrawnObject{
//    QVector<int*> vector;
    drawspace* view;
public:
    QVector<int*> vector;
    QVector<int*> vertices;
    QVector<int*> vertices2;
    QVector<float> speeds;
    QVector<int*> cleanedVertices;
    DrawnObject(drawspace* view);
    void addData(int x, int y, int time);
    ~DrawnObject();
    void clean();
    void analyze();
    float speedCalc(int index);
    void analyzeWithSlopes(int gap);
    int binarySearch(int start, int end, float slope, float tolerence);
    void drawVertices(QVector<int*> vertices, QPen pen);
    QVector<int*> cleanupVertices(QVector<int*> vertices);
};

#endif // DRAWNOBJECT_H
