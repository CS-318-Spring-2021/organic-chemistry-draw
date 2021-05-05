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
    QPushButton *saveButton;
    QCheckBox *recordCheckBox;
    QCheckBox *undoCheckBox;
    QPushButton *undoButton;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void bSave();
    void bRecording();
    void bUndo();
};
#endif // MAINWINDOW_H
