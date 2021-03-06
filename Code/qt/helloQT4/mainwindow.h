/*
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "winslam.h"
#include "datadialogscalatraslarota.h"
#include "datadialogshowestimated.h"
#include "dialogscalatraslarota.h"
#include "dialogshowestimated.h"
#include "transformador2/Transformador.h"
#include "Registrador/Registrador.h"
#include "GeneratorPCA/GeneratorPCA.h"
#include "ModuloEscala/FindScala.h"
#include "AjusteTiempo/AjusteTiempo.h"
#include "Interpolator/Interpolator.h"
#include "Point3D.h"
#include "Eigen/Dense"
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class GLWidget;
class QDialog;
//class Point3D;

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow();
    void loadFile(const QString &fileName);
    void setScala(double X, double Y, double Z);
    void setTrasla(double X, double Y, double Z);
    void performModifySequence(double scalaX,double scalaY,double scalaZ, double traslaX,double traslaY, double traslaZ,double rotaX,double rotaY,double rotaZ,double gNoise,double cNoise, double timeOffset,int pcaIndex,double frequency);
    QWidget *myWinSlam;



    //DIALOG that shows parameters of transformation to be performed , scale, traslation , rotation, gaussian and cosmic noise
    DialogScalaTraslaRota *dialogScalaTraslaRota;
    DataDialogScalaTraslaRota* dataDialogScalaTraslaRota;

    //DIALOG that show parameters of ESTIMATED transformations, scale, traslation, rotation
    DialogShowEstimated *dialogShowEstimated;
    DataDialogShowEstimated *dataDialogShowEstimated;

    //============================== Model Objects to estimate transformation
    Transformador myTransformador; //Perform transformations (Scale,Rotation,Traslation,Gaussian and Cosmic Noise ) over dataset
    Registrador myRegistrador;  //Estimate Rotation and Traslation over a dataset
    GeneratorPCA myGeneratorPCA;//Calculate PCA over a dataset
    FindScala myFindScala; // to find the scala
    AjusteTiempo myFindOffset; //to find timeOffset
    Interpolator myInterpolator;// to interpolate
    char * myInputFileName;
    char * myOutputFileName;
    //==============================

    //============================== Matrix that will store the data model
    Eigen::MatrixXd readingA;
    Eigen::MatrixXd readingB;
    Eigen::MatrixXd readingAbkp;
    Eigen::MatrixXd readingBbkp;
    Eigen::MatrixXd dataEstimated;
    Eigen::MatrixXd rotationEstimated;
    Eigen::MatrixXd traslationEstimated;
    double timeA [3000];
    double timeB [3000];

    //===============================
    double timeOffset;
    double timeOffsetEstimated;
    double rMax;
    double frequency;
    int ftype;

    //Point3D * myScala;
private slots:
    void onOpenFile();
    void onModifySequence();
    void onEstimateSequence(int way); // way==0 means A to B .  way==1 means B to A
    void onEstimateSequenceAtoB();
    void onEstimateSequenceBtoA();
    void onExit();
    void onSetDots();//Menu View:indicate that dataset will be displayed as 3d points
    void onSetLines();//Menu View:indicate that dataset will be displayed as 3d lines
    void onViewJustEstimated();//Menu View; indicate that only estimated dataset will be shown
protected:
    //Winslam myWinSlam;

};

#endif
