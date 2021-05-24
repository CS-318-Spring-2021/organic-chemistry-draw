#include "MainWindow.h"

#include <QtWidgets>
#include <QPrinter>
#include <QDebug>
#include <stdio.h>
#include <QPrintDialog>

#include "Drawspace.h"
#include "Molecule.h"
#include "QAtom.h"
#include "QBond.h"

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

    rightLayout->addWidget(aboutButton = new QPushButton("About"));
    connect(aboutButton, &QPushButton::clicked, this, &MainWindow::bAbout);

    mainLayout->addWidget(view = new Drawspace(), 1);
    mainLayout->addLayout(rightLayout);
    bRecording();
}

MainWindow::~MainWindow(){}

void MainWindow::bSave() {
    QString date = QDate::currentDate().toString("MM-dd-yyyy");
    QString fName = QFileDialog::getSaveFileName(this, "Enter save filename", date, tr("Image File (*.pdf)"));
    if (fName.isEmpty()) return;
    QFile fOut(fName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error opening file", "Can't open selected file for output.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fName);
    if(!fName.isEmpty()) {
        QPainter p(&printer);
        view->render(&p);
        p.end();
    }
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
    if(view->undoStackMolecule.size() >= 1) view->drawExisting();
}

void MainWindow::bClear(){
    QVector<Molecule*> empty;
    view->undoStackMolecule.append(empty);
    QVector<DrawnObject*> emptyDrawnObject;
    view->undoStackDrawnObject.append(emptyDrawnObject);
    view->molecules.clear();
    view->freeHandObjects.clear();
    view->bondLength = -1;
    view->mScene.clear();
}

void MainWindow::bAbout(){
    QMessageBox msg;
    msg.setWindowTitle("About");
    msg.setText(""
"About the tool:\n"
"Existing organic chemistry drawing tools are designed to create pretty and "
"professional structures that are ready for academic papers. While they give "
"users the opportunity to draw a wide range of structures, they are slow and "
"difficult to navigate. Our tool is less expansive, but more user friendly. It "
"was designed with teachers and students in mind, allowing for quick notetaking with a small learning curve.\n\n"

"To make the most out of this tool, draw structures as you normally would, "
"and our code will clean up your drawings. To cycle through bond types "
"(including dashed and wedge bonds), simply click on the bond you want to "
"change. Checking the “free draw” box turns off corrections so you can label "
"atoms and draw arrows. When you are all done, you can clear your drawing, or "
"you can save your masterpiece as a PDF.\n\n"

"About the project:\n"
"We are a group of Computer Science and Biochemistry majors at Middlebury "
"College and we created this tool in the spring semester of 2021 in CS 318 – "
"OOP & GUI Application Dev. It was the idea of Professor Jeff Byers and our "
"class was taught by Professor Frank Swenton.\n\n"

"Credits:\n"
"Ammar Almahdy, Hannah Brady, Isabelle Cochran, Sam de Wolf, Zack Einhorn, "
"Charlotte Gray, Aska Matsuda, Alec Wilson");
    msg.exec();
}
