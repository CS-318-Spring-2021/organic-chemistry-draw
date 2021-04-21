#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <drawspace.h>
#include <QWidget>
#include "DrawnObject.h"
#include "Molecule.h"


class PollingGraphicsView;
class QTableWidget;
class QPushButton;
class QCheckBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    drawspace *view;
    QTableWidget *table;
    QPushButton *saveButton;
    QCheckBox *recordCheckBox;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DrawnObject *currentDrawnObject;
    bool recording;
    QVector<Molecule*> molecules;

public slots:
    void onMouseEvent(int type, int when, QPointF scenePos);
    void bSave();
    void bRecording();
    void updateCrosshairs(int curRow, int curCol, int prevRow, int prevCol);
};
#endif // MAINWINDOW_H
