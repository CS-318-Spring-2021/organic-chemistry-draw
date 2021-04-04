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
    DrawnObject(drawspace* view);
    void addData(int x, int y, int time);
    ~DrawnObject();
    void clean();
    void analyze();
    float speedCalc(int index);
    void analyzeWithSlopes(int gap);
    int binarySearch(int start, int end, float slope, float tolerence);
};

#endif // DRAWNOBJECT_H
