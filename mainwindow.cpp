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

    rightLayout->addWidget(aboutButton = new QPushButton("About"));
    connect(aboutButton, &QPushButton::clicked, this, &MainWindow::bAbout);

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

void MainWindow::bAbout(){
    QMessageBox msg;
    msg.setWindowTitle("About");
    msg.setText(""
"About the Project \n     We were motivated to pursue this project because we wanted to build something simpler and more convenient than the existing technology. "
"Programs like ChemDraw allow wider varieties of structures to be drawn, but they rely on a combination of large, comprehensive menu bars "
"and dragging the molecules onto the screen, which is slow to navigate and less convenient than simply drawing. Our program is less expansive, "
"but more user friendly. \n     Users interact with our tool by drawing. When a user creates an organic chemistry structure in our program, it will "
"automatically clean itself up into common structures such as hexagons and line structures. It also supports free drawing (check the “free draw” box)"
" for when writing needs to be added but not corrected. Users also have the ability to undo their mistakes, clear their drawings, or save the canvas "
"as a PNG.\n\nList of Contributors:\nAmmar Almahdy, Hannah Brady, Isabelle Cochran, Sam de Wolf, Zack Einhorn, Charlotte Gray, Aska Matsuda, Alec Wilson.\n"
"\nWe'd like to thank Professor Frank Swenton for teaching this class and assisting us along the way, as well as Professor Jeff Byers for the idea."
"\n\nMiddlebury College Computer Science Students, 2021");
    msg.exec();
}
