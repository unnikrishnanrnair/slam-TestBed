/*
 * #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/

#include "mainwindow.h"
#include "window.h"
#include "winslam.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <QtWidgets>
#include <QDialog>
#include "Eigen/Dense"
#include "transformador2/Transformador.h"
//#include "transformador2/Point3D.h"
//#include <QGLWidget>


MainWindow::MainWindow()
{
    int maxLine = 3000;
    Eigen::MatrixXd newMatrixA (maxLine,3);
    readingA = newMatrixA;
    Eigen::MatrixXd newMatrixB (maxLine,3);
    readingB = newMatrixB;
    Eigen::MatrixXd newMatrixEstimated (maxLine,3);
    dataEstimated = newMatrixEstimated;
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu(tr("&File"));
    QAction *openFile = new QAction(menuWindow);
    openFile->setText(tr("Open"));
    menuWindow->addAction(openFile);
    connect(openFile, &QAction::triggered, this, &MainWindow::onOpenFile);
    QAction *exitApp = new QAction(menuWindow);
    exitApp->setText(tr("Exit"));
    menuWindow->addAction(exitApp);
    connect(exitApp, &QAction::triggered, this, &MainWindow::onExit);

    QMenu *menuSequence = menuBar->addMenu(tr("&Sequence Manager"));
    QAction *modifySequence = new QAction(menuWindow);
    modifySequence->setText(tr("Modify sequence"));
    menuSequence->addAction(modifySequence);
    connect(modifySequence, &QAction::triggered, this, &MainWindow::onModifySequence);

    QMenu *menuEstimate = menuBar->addMenu(tr("&Estimator"));
    QAction *estimateSequence = new QAction(menuWindow);
    estimateSequence->setText(tr("Estimate sequence"));
    menuEstimate->addAction(estimateSequence);
    connect(estimateSequence, &QAction::triggered, this, &MainWindow::onEstimateSequence);

    QMenu *menuView = menuBar->addMenu(tr("&View"));
    QAction *setDots = new QAction(menuWindow);
    setDots->setText(tr("3d dots"));
    menuView->addAction(setDots);
    connect(setDots, &QAction::triggered, this, &MainWindow::onSetDots);
    QAction *setLines = new QAction(menuWindow);
    setLines->setText(tr("3d lines"));
    menuView->addAction(setLines);
    connect(setLines, &QAction::triggered, this, &MainWindow::onSetLines);


    setMenuBar(menuBar);

    //setMenuBar(menuBar);

    if (!centralWidget())
        //setCentralWidget(new Window(this));
        myWinSlam=(QWidget*)(new Winslam(this));
        setCentralWidget(myWinSlam);
    //onAddNew();
    dataDialogScalaTraslaRota= new DataDialogScalaTraslaRota(1.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);

}

void MainWindow::onExit()
{
    this->close();
}
void MainWindow::onSetDots()
{
    ((Winslam*)(myWinSlam))->setDots();
}
void MainWindow::onSetLines()
{
    ((Winslam*)(myWinSlam))->setLines();
}

void MainWindow::onOpenFile()
{
    /*if (!centralWidget())
        setCentralWidget(new Window(this));
    else
        QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
    */
    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
    QString fileName = QFileDialog::getOpenFileName(this);
    std::string fname = fileName.toStdString();
    char * cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    myInputFileName= cstr;
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::onModifySequence()
{
     std::cout<< "onModifySequence" <<std::endl;
    //dialogScalaTraslaRota =(QDialog*) (new DialogScalaTraslaRota (this));
    dialogScalaTraslaRota =(new DialogScalaTraslaRota (this));
    //dialogScalaTraslaRota->setDataDialog(new DataDialogScalaTraslaRota(1.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0));
    dialogScalaTraslaRota->setDataDialog(dataDialogScalaTraslaRota);
    dialogScalaTraslaRota->show();
    dataDialogScalaTraslaRota= dialogScalaTraslaRota->getDataDialog();
    std::cout<<"onModifySequence.dataDialogScalaTraslaRota->getCosmicNoiseDeviation()"<<dataDialogScalaTraslaRota->getCosmicNoiseDeviation();
    std::cout<< "FIN onModifySequence" <<std::endl;
}

void MainWindow::onEstimateSequence(){
    std::cout<<"onEstimateSequence"<<std::endl;
    //myRegistrador.rigid_transform_3D()

    int contLin = readingB.rows();

    //Begin Try to estimate Scala using PCA
    //=========================================
    MatrixXd AA,BB,AApca,BBpca (contLin,3);
    MatrixXd pcaA,pcaB;
    //B = readingB.block(0,0,contLin,3);
    AA = readingA.block(0,0,readingA.rows(),3);//to use with Scale and PCA
    BB = readingB.block(0,0,readingB.rows(),3);//to use with Scale and PCA

    myGeneratorPCA.calculatePCAbySVD(0,AA, AApca, pcaA);//A is converted to PCA. Important,inside this function also is calculated A.rowwise() - A.colwise().mean(). A is converted to a newA
    myGeneratorPCA.calculatePCAbySVD(0,BB, BBpca, pcaB);//B is converted to PCA. Important,inside this function also is calculated B.rowwise() - B.colwise().mean(). B is converted to a newB

    Vector3d myScalaSVD = myFindScala.getScalaSVD(AApca,BBpca);
    std::cout <<"MainWindow::onEstimateSequence myScalaSVD="<<myScalaSVD<<std::endl;

    // End Try to estimate Scala using PCA
    //==========================================


    // Adapt dataB to Scale if necessary
    //==========================================

    if (myScalaSVD(0) > 1 || myScalaSVD(1) > 1 || myScalaSVD(1) > 1 ){ //if myScaleSVD is not (1,1,1)

        MatrixXd newB(readingB.rows(),3);
        newB = readingB.block(0,0,readingB.rows(),3);
        std::cout <<"Scale is greater than 1. Divide by Scale dataset B and datasetBB"<<myScalaSVD<<std::endl;
        for (int i= 0; i< newB.rows(); i++){
            VectorXd aRow = newB.row(i);
            newB.row(i) << aRow(0)/myScalaSVD(0),aRow(1)/myScalaSVD(1),aRow(2)/myScalaSVD(2);
         }

        myRegistrador.rigid_transform_3D(newB,readingA,rotationEstimated,traslationEstimated);
        std::cout<< "MainWindow::onEstimateSequence rotationEstimated="<<rotationEstimated <<std::endl;
        std::cout<< "MainWindow::onEstimateSequence traslationEstimated="<<traslationEstimated <<std::endl;
        myRegistrador.applyTransformationsOverData(newB,dataEstimated,rotationEstimated,traslationEstimated);
    } else{

        myRegistrador.rigid_transform_3D(readingB,readingA,rotationEstimated,traslationEstimated);
        std::cout<< "MainWindow::onEstimateSequence rotationEstimated="<<rotationEstimated <<std::endl;
        std::cout<< "MainWindow::onEstimateSequence traslationEstimated="<<traslationEstimated <<std::endl;
        myRegistrador.applyTransformationsOverData(readingB,dataEstimated,rotationEstimated,traslationEstimated);
    }




    //std::cout<< "MainWindow::onEstimateSequence rotationEstimated="<<rotationEstimated <<std::endl;
    ((Winslam*)(myWinSlam))->setEstimatedDataView(dataEstimated);
    dialogShowEstimated =(new DialogShowEstimated (this));
    //dialogScalaTraslaRota->setDataDialog(new DataDialogScalaTraslaRota(1.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0));
    double x1 = rotationEstimated.row(0)(0);
    double y1 = rotationEstimated.row(0)(1);
    double z1 = rotationEstimated.row(0)(2);

    double x2 = rotationEstimated.row(1)(0);
    double y2 = rotationEstimated.row(1)(1);
    double z2 = rotationEstimated.row(1)(2);

    double x3 = rotationEstimated.row(2)(0);
    double y3 = rotationEstimated.row(2)(1);
    double z3 = rotationEstimated.row(2)(2);

    dataDialogShowEstimated= new DataDialogShowEstimated(myScalaSVD(0),myScalaSVD(1),myScalaSVD(2),traslationEstimated(0),traslationEstimated(1),traslationEstimated(2),x1,y1,z1,x2,y2,z2,x3,y3,z3);

    dialogShowEstimated->setDataDialog(dataDialogShowEstimated);
    dialogShowEstimated->show();

}
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    int maxLine = 3000;
    //Eigen::MatrixXd readingA (maxLine,3);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    } else{
        std::cout<< "leyendo archivo" <<std::endl;
        std::ifstream infileA( fileName.toStdString());
        double timestamp,rx,ry,rz,q1,q2,q3,q4=0;
        int contLin=0;

        while ( (infileA >> timestamp >> rx >> ry >> rz >> q1 >> q2 >> q3 >> q4) && contLin<maxLine ){
                    std::cout <<"contLin="<<contLin<<std::endl;
                    readingA.row(contLin)<< rx,ry,rz;
                    std::cout <<"contLin="<<contLin<<std::endl;
                    contLin ++;


        }
        infileA.close();

        std::cout <<"fin leyendo Archivo"<<std::endl;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    ((Winslam*)(myWinSlam))->setDataView(readingA);
}

void MainWindow::setScala(double X, double Y, double Z){
     std::cout<< "MainWindow.setScala" <<std::endl;
    ((Winslam*)(myWinSlam))->setScala(X,Y,Z);
}

void MainWindow::setTrasla(double X, double Y, double Z){
    ((Winslam*)(myWinSlam))->setTrasla(X,Y,Z);
    //performModifySequence(X,Y,Z, X,Y, Z,0,0,0,0,0);
}

void MainWindow::performModifySequence(double scalaX,double scalaY,double scalaZ, double traslaX,double traslaY, double traslaZ,double rotaX,double rotaY,double rotaZ,double gNoise, double cNoise){
     dataDialogScalaTraslaRota->setScaleX(scalaX);
     dataDialogScalaTraslaRota->setScaleY(scalaY);
     dataDialogScalaTraslaRota->setScaleZ(scalaZ);
     dataDialogScalaTraslaRota->setTraslaX(traslaX);
     dataDialogScalaTraslaRota->setTraslaY(traslaY);
     dataDialogScalaTraslaRota->setTraslaZ(traslaZ);
     dataDialogScalaTraslaRota->setRotaX(rotaX);
     dataDialogScalaTraslaRota->setRotaY(rotaY);
     dataDialogScalaTraslaRota->setRotaZ(rotaZ);
     dataDialogScalaTraslaRota->setGaussianNoiseDeviation(gNoise);
     dataDialogScalaTraslaRota->setGaussianNoiseDeviation(cNoise);


     int maxLine = 3000;
     Point3D  myScala;
     myScala.setXYZ(scalaX,scalaY,scalaZ);

     Point3D miTraslacion ;
     miTraslacion.setXYZ(traslaX,traslaY,traslaZ);

     myOutputFileName="miSalidaContaminadaQT.txt";
     std::cout <<"mainwindow.performModifySequence  gNoise="<<gNoise<< " cNoise="<<cNoise<<std::endl;
     char axisToRotate='X';
     double rotationGrades = 0;

     if (rotaX>0) {
         rotationGrades = rotaX;
         axisToRotate = 'X';

     }else if (rotaY>0){
         rotationGrades = rotaY;
         axisToRotate = 'Y';
     }else if (rotaZ>0){
         rotationGrades = rotaZ;
         axisToRotate = 'Z';
     }else {
         rotationGrades = 0;
     }


     if (gNoise > 0.0 & cNoise >0.0) {
         myTransformador.createContaminatedSequence(myInputFileName,myOutputFileName,miTraslacion,myScala,rotationGrades,'X',1,1,0.0);
     } else if (gNoise > 0.0 & cNoise <=0.0) {
        myTransformador.createContaminatedSequence(myInputFileName,myOutputFileName,miTraslacion,myScala,rotationGrades,'X',1,0,0.0);
     } else if (gNoise <= 0 & cNoise >0.0){
         myTransformador.createContaminatedSequence(myInputFileName,myOutputFileName,miTraslacion,myScala,rotationGrades,'X',0,1,0.0);
     } else  myTransformador.createContaminatedSequence(myInputFileName,myOutputFileName,miTraslacion,myScala,rotationGrades,'X',0,0,0.0);

     // Reading input file B, with new dataset contaminated
     std::ifstream infileB( "miSalidaContaminadaQT.txt" );
     //Eigen::MatrixXd readingB (maxLine,3);
     double timestamp,rx,ry,rz,q1,q2,q3,q4=0;
     int contLin=0;

     while ( (infileB >> timestamp >> rx >> ry >> rz >> q1 >> q2 >> q3 >> q4) && contLin<maxLine ){
                         readingB.row(contLin)<<rx,ry,rz;

                         contLin ++;

      }
     infileB.close();
     ((Winslam*)(myWinSlam))->setContaminatedDataView(readingB);


}


