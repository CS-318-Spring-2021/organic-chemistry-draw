#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>
#include <stdio.h>

#include "drawspace.h"
#include "Molecule.h"
#include "qatom.h"
#include "qbond.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    QVector<Molecule*> molecules;
    QWidget *w = new QWidget();
    setCentralWidget(w);

    QHBoxLayout *mainLayout = new QHBoxLayout(w);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(saveButton = new QPushButton("Save"));


    connect(saveButton, &QPushButton::clicked, this, &MainWindow::bSave);

    QVBoxLayout *taskBarLayout = new QVBoxLayout();
    taskBarLayout->addWidget(recordCheckBox = new QCheckBox("Record"));

    recordCheckBox->setCheckable(true);
    recordCheckBox->setChecked(true);

    connect(recordCheckBox, &QCheckBox::stateChanged, this, &MainWindow::bRecording);

    rightLayout->addWidget(undoButton = new QPushButton("Undo"));
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::bUndo);

    rightLayout->addWidget(clearButton = new QPushButton("Clear"));
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::bClear);

    mainLayout->addLayout(taskBarLayout);
    mainLayout->addWidget(view = new drawspace(), 1);
    mainLayout->addLayout(rightLayout);
    bRecording();


}

MainWindow::~MainWindow()
{

}

void MainWindow::bSave() {
    QString date = QDate::currentDate().toString("MM-dd-yyyy");
    QString fName = QFileDialog::getSaveFileName(this, "Enter save filename", date, tr("Image File (*.png)"));
    if (fName.isEmpty()) return;
    QFile fOut(fName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error opening file", "Can't open selected file for output.");
        return;
    }

    QPixmap pixMap = view->grab();
    pixMap.save(fName);
    return;
}

void MainWindow::bRecording(){
    view->recording = recordCheckBox->isChecked();
}

void MainWindow::bUndo(){
    if(view->undoStackMolecule.size() <= 1) {
        view->undoStackMolecule.clear();
        view->molecules.clear();
        view->mScene.clear();
        return;
    }
    view->undoStackMolecule.removeLast();
    QVector<Molecule*> deepCopy = view->undoStackMolecule.last();
    view->molecules = deepCopy;
    view->molecules = view->makeMoleculesFreshCopy();
    view->mScene.clear();
    if(view->undoStackMolecule.size() >= 1) view->drawExisting();
}

void MainWindow::bClear(){
    QVector<Molecule*> empty;
    view->undoStackMolecule.append(empty);
    view->molecules.clear();
    view->mScene.clear();
}
