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
    void analyzeWithSlopes();
    int binarySearch(int start, int end, float slope, float tolerence);
    void drawVerticesx(QVector<int*> vertices, QPen pen);
    void drawVerticesy(QVector<int*> vertices, QPen pen);

    void runTests();
    QVector<int*> cleanupVertices(QVector<int*> vertices);

};

#endif // DRAWNOBJECT_H
