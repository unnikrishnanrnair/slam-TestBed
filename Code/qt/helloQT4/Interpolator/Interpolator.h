#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H
#include <iostream>
#include "Eigen/Dense"
//#include "../AjusteTiempo/AjusteTiempo.h"
using namespace Eigen;
//using namespace std;

class Interpolator {
private:
	MatrixXd m;

public:
   Interpolator ();
   //void genera2Series(int maxLine, double proporcionFrecuencia,double offset, MatrixXd& A, MatrixXd& B);
   //void calcularAutocorrelacion(int maxLine,int intervalo, double offset, MatrixXd&A, MatrixXd&B);
   //void calcularAutocorrelacion2(int maxLine,int intervalo, double offset, MatrixXd&A, MatrixXd&B);
   //void calcularAutocorrelacion3(char coordinate, int maxLine, int intervalo,double offset, MatrixXd& A1, MatrixXd& B2);
   void interpolate(int maxLine, MatrixXd& A, MatrixXd& B);
   void reduceSequence (int maxLine, MatrixXd& aMatrix, int numberToDelete);
   void reduceSequence (int step, MatrixXd& aMatrix);
   //MatrixXd insertRowInSequence (MatrixXd& C, VectorXd myVector, int position);
   void insertRowInSequence (MatrixXd& C, VectorXd myVector, int position);
   double interpolateValue (double x,double x2,double y2,double x3,double y3);
   void interpolate2Series(int maxLine, MatrixXd& A, MatrixXd& B);
   void modifyTime (double valueToAdd, MatrixXd& aMatrix);
   void interpolate2SeriesB(int maxLine, MatrixXd& A, MatrixXd& B);
   MatrixXd interpolateAoverB(MatrixXd& A, MatrixXd& B);
   void interpolateSerieToFrequency(float frequency, MatrixXd& B);
   void interpolateSerieToFrequency2(float frequency, MatrixXd& B);
   double calculateOffsetWithInterpolation(int maxLine, int interval, double& offsetEstimated, MatrixXd A1,MatrixXd B2);
   void interpolate2SeriesFMin(int maxLine, MatrixXd& A, MatrixXd& B);
   void performInterpolation(int freqType, double freq, MatrixXd& A, MatrixXd& B);
   double calculateOffsetWithInterpolation2(MatrixXd A, MatrixXd B, float& rMax);
   double calculateOffsetWithInterpolation3(MatrixXd A, MatrixXd B);
   int timeLessThan (double time1, double time2); // to compare float time converted to long integer
   int timeEqualThan (double time1, double time2); // to compare float time converted to long integer
   void traza (double time1, double time2); // to compare float time converted to long integer
};
#endif
