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


    rightLayout->addWidget(recordCheckBox = new QCheckBox("Free Draw"));

    recordCheckBox->setCheckable(true);
    recordCheckBox->setChecked(false);

    connect(recordCheckBox, &QCheckBox::stateChanged, this, &MainWindow::bRecording);
    rightLayout->addWidget(undoButton = new QPushButton("Undo"));
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::bUndo);

    rightLayout->addWidget(clearButton = new QPushButton("Clear"));
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::bClear);

    rightLayout->addWidget(saveButton = new QPushButton("Save"));


    connect(saveButton, &QPushButton::clicked, this, &MainWindow::bSave);
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
        view->undoStackDrawnObject.clear();
        QVector<Molecule*> empty;
        view->undoStackMolecule.append(empty);
        QVector<DrawnObject*> emptyDO;
        view->undoStackDrawnObject.append(emptyDO);
        view->molecules.clear();
        view->freeHandObjects.clear();
        view->mScene.clear();
        return;
    }
    view->undoStackMolecule.removeLast();
    view->undoStackDrawnObject.removeLast();
    QVector<Molecule*> deepCopy = view->undoStackMolecule.last();
    view->molecules = deepCopy;
    view->molecules = view->makeMoleculesFreshCopy();
    QVector<DrawnObject*> freeHandCopy = view->undoStackDrawnObject.last();
    view->freeHandObjects = freeHandCopy;
    view->freeHandObjects = view->makeDrawnObjectsFreshCopy();
    view->mScene.clear();
    if(view->undoStackMolecule.size() >= 1) {
        view->drawExisting();

    }
}

void MainWindow::bClear(){
    QVector<Molecule*> empty;
    view->undoStackMolecule.append(empty);
    QVector<DrawnObject*> emptyDO;
    view->undoStackDrawnObject.append(emptyDO);
    view->molecules.clear();
    view->freeHandObjects.clear();
    view->bondLength = -1;
    view->mScene.clear();
}
