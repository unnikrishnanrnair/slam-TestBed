#include <iostream>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <sys/time.h>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include "Eigen/Dense"
#include "Eigen/SVD"
using namespace Eigen;
#include <limits>
#include <stdbool.h>
//#include "../RegistradorHorn/RegistradorHorn.h"
#include "FindScala.h"


//#include "Eigen/Dense"

FindScala::FindScala(){
	std::cout<< "constructor por defecto" <<std::endl;
}
void FindScala::getScalaRansac(MatrixXd A, MatrixXd B, int contLin) {
	int maxIterations = 10;
	int iterations = 0;

    Vector3d prevScala(-1,-1,-1);
    int contScala=0;
	while (iterations < maxIterations) { //startin Ransac Algorithm
		 iterations ++;
		//std::cout << "B "<<B<<std::endl;
		int nvalores = 20;
		// Select 30 points inliers,
		MatrixXd mInliersA (nvalores,3);
		MatrixXd mInliersB (nvalores,3);
		int i=0;
		bool visitados[contLin] ={ false};
		int aInlierIdx =0;
		while (i < nvalores) { // Seleccionamos aleatoriamente los primeros supuestos inliers
			            aInlierIdx=1+(int) (3000.0*rand()/(RAND_MAX+1.0));
					//if (visitados[aInlierIdx]==false) {
						visitados[aInlierIdx]=true;
						//std::cout<<i << " aInlierIdx "<< aInlierIdx	<<std::endl;
						mInliersA.row(i)= A.row(aInlierIdx);
						mInliersB.row(i)= B.row(aInlierIdx);
						i++;
					//}

		}
		//Hallar escala

		MatrixXd AA = mInliersA.rowwise() - mInliersA.colwise().mean();
		MatrixXd BB = mInliersB.rowwise() - mInliersB.colwise().mean();
		Vector3d myScala = getScalaEigenValues( AA,  BB);
		//std::cout <<"scalaX="<<myScala(0) <<"preScalaX="<<prevScala(0)<<std::endl;
		//std::cout <<"scalaY="<<myScala(1) <<"preScalaY="<<prevScala(1)<<std::endl;
		//std::cout <<"scalaZ="<<myScala(2) <<"preScalaZ="<<prevScala(2)<<std::endl;
		double dif0 = myScala(0)- prevScala(0);
		double dif1 = myScala(1)- prevScala(1);
		double dif2 = myScala(2)- prevScala(2);

		if ((dif0 < 0.0001)  && (dif1 < 0.0001)  && (dif2 < 0.0001)){
			contScala ++;

		} else {
			prevScala=myScala;
			contScala=0;
		}

		if (contScala == 5){
			//std::cout<<i << " ++++++++++++++++++++++++++He encontrado  coincidencias de scala "<< contScala	<<std::endl;
			iterations = maxIterations ; // salgo del bucle

		}
		//quedarnos con el mejor resultado


	}
	//std::cout<< " ++++++++++++++++++++++++++FinBucle "<< contScala	<<std::endl;
	return;

}

Vector3d FindScala::getScalaEigenValues(MatrixXd AA, MatrixXd BB){
	/// method eigen values

		//MatrixXd centered = mat.rowwise() - mat.colwise().mean();
		//MatrixXd cov = centered.adjoint() * centered;
		MatrixXd AAcov = AA.adjoint() * AA;

		//and then perform the eigendecomposition:

		SelfAdjointEigenSolver<MatrixXd> eigA(AAcov);


		//eigenvalues

		std::cout << "eig.eigenvalues() \n"<< eigA.eigenvalues() << std::endl;
		Vector3d eigenValuesA=eigA.eigenvalues() ;

		MatrixXd BBcov = BB.adjoint() * BB;

				//and then perform the eigendecomposition:

		SelfAdjointEigenSolver<MatrixXd> eigB(BBcov);


		//eigenvalues

		std::cout << "eig.eigenvalues() \n"<< eigB.eigenvalues() << std::endl;

		std::cout << "Function getScalaEigenValues that estimates Scale WITH EIGEN VALUES \n"<<  std::endl;
		Vector3d eigenValuesB=eigB.eigenvalues() ;
		double scalaX = sqrt(eigenValuesB(0)  / eigenValuesA(0)) ;
		double scalaY = sqrt(eigenValuesB(1)  / eigenValuesA(1)) ;
		double scalaZ = sqrt(eigenValuesB(2)  / eigenValuesA(2)) ;

		std::cout <<"scalaX="<<scalaX <<std::endl;
		std::cout <<"scalaY="<<scalaY <<std::endl;
		std::cout <<"scalaZ="<<scalaZ <<std::endl;
		Vector3d myScala (scalaX,scalaY,scalaZ);
		return myScala;



}

Vector3d FindScala::getScalaSVD(MatrixXd AA, MatrixXd BB){

	//JacobiSVD<MatrixXd> svd(AA,ComputeFullU | ComputeFullV);
	JacobiSVD<MatrixXd> svd(AA,Eigen::ComputeThinV);
			//MatrixXd W = svd.matrixV().leftCols(3);
		//	MatrixXd V1 = svd.matrixV();
	    //	std::cout << "V1 \n"<< V1 << std::endl;
		//	MatrixXd U1 = svd.matrixU();
			//std::cout << "U1 \n"<< U1 << std::endl;
			MatrixXd S1 = svd.singularValues();
			std::cout << "S1 \n"<< S1 << std::endl;
			//MatrixXd W = svd.matrixV().leftCols(3);
			//std::cout << "W \n"<< W << std::endl;
			//MatrixXd R1 = V1*S1;
		//	MatrixXd R1 = V1.transpose()*S1;
		//	std::cout << "R1 \n"<< R1 << std::endl;
			//MatrixXd X1 = U1*S1;
			//std::cout << "X1 \n"<< X1 << std::endl;

			//JacobiSVD<MatrixXd> svd2(BB,ComputeFullU | ComputeFullV);
			JacobiSVD<MatrixXd> svd2(BB,Eigen::ComputeThinV);
			//MatrixXd Z = svd2.matrixV().leftCols(3);
		//	MatrixXd V2 = svd2.matrixV();
		//	std::cout << "V2 \n"<< V2 << std::endl;
		//	MatrixXd U2 = svd2.matrixU();
			//std::cout << "U2 \n"<< U2 << std::endl;
			MatrixXd S2 = svd2.singularValues();
			std::cout << "S2 \n"<< S2 << std::endl;
			//MatrixXd Z = svd2.matrixV().leftCols(3);
			//std::cout << "Z \n"<< Z << std::endl;

			//MatrixXd R2 = V2*S2.transpose();
		//   MatrixXd R2 = V2.transpose()*S2;
		//	std::cout << "R2 \n"<< R2 << std::endl;

			std::cout << "SCALE ESTIMATED WITH SDV \n"<<  std::endl;

			double scalaX = S2(0)  / S1(0) ;
			double scalaY = S2(1)  / S1(1) ;
			double scalaZ = S2(2)  / S1(2) ;

			std::cout <<"scalaX="<<scalaX <<std::endl;
			std::cout <<"scalaY="<<scalaY <<std::endl;
			std::cout <<"scalaZ="<<scalaZ <<std::endl;
			Vector3d myScala (scalaX,scalaY,scalaZ);
			return myScala;
}

//int main( int argc, char** argv )
void principal()
{
	//std::cout << std::setprecision(6) << std::fixed;

    std::ofstream out( "/home/tfm3/workspace/ModuloScala/miSalidaRegistro.txt" );
    out << std::setprecision(6) << std::fixed;
	//std::ifstream infile("/home/tfm3/workspace/ModuloEscala/miEntrada.txt");
    std::ifstream infile("/home/tfm3/workspace/SLAMTestBed/miEntradaA.txt");
	//std::string line;
	//infile >> std::setprecision(6) >> std::fixed;
	//MatrixXd A (3000,3);
	std::cout <<"antes de declarar la variable"<<std::endl;
	MatrixXd readingA (6000,3);//size is big enough to store an unknown number of data rows from file
	//MatrixXd readingA;
	int contLin=0;
	double timestamp,rx,ry,rz,q1,q2,q3,q4=0;
	std::cout <<"antes de leer el archivo"<<std::endl;
	while ( infile >> timestamp >> rx >> ry >> rz >> q1 >> q2 >> q3 >> q4 ){
	//while ( infile >> rx >> ry >> rz  ){
			//std::cout << "You said.... contLin"<< contLin << timestamp << " " << rx << " " << ry << " " << rz << " " << q1 << " " << q2 << " " << q3 << " " << q4 << "\n";
			//VectorXd myVector (rx,ry,rz);
			//readingA.row(contLin)= myVector;
			readingA.row(contLin)<< rx,ry,rz;
			contLin ++;

	}
	infile.close();
	//std::cout <<"contLin="<<contLin<<std::endl;
	MatrixXd A (contLin,3);
	//A = readingA.block<contLin,3>(0,0);
	A = readingA.block(0,0,contLin,3);


	std::cout << ">>>>>>>CIERRO INFILE\n"<<std::endl;

	//std::ifstream infile2("/home/tfm3/workspace/ModuloEscala/miEntradaModificada.txt");
	//std::ifstream infile2("/home/tfm3/workspace/ModuloEscala/miEntrada.txt");
	std::ifstream infile2("/home/tfm3/workspace/SLAMTestBed/miSalidaContaminada.txt");
	//std::string line;
    //infile >> std::setprecision(6) >> std::fixed;
	//MatrixXd A (3000,3);
	std::cout <<"antes de declarar la variable"<<std::endl;
	MatrixXd readingB (6000,3);//size is big enough to store an unknown number of data rows from file
	//MatrixXd readingA;
	contLin=0;
	timestamp=0,rx=0,ry=0,rz=0,q1,q2,q3,q4=0;
	std::cout <<"antes de leer el archivo"<<std::endl;
	while ( infile2 >> timestamp >> rx >> ry >> rz >> q1 >> q2 >> q3 >> q4 ){
	//while ( infile2 >> timestamp >> rx >> ry >> rz >> q1 >> q2 >> q3 >> q4 ){
	        //readingB.row(contLin)<< 2*rx,10*ry,4*rz;// As example, when reading the second file, a scale (2,10,4) is applied over every x,y,z
		    //readingB.row(contLin)<< 120*rx,17*ry,11*rz;// As example, when reading the second file, a scale (2,10,4) is applied over every x,y,z
		    //readingB.row(contLin)<< 2*rx,2*ry,2*rz;// As example, when reading the second file, a scale (2,10,4) is applied over every x,y,z
		readingB.row(contLin)<< 1*rx,1*ry,1*rz;// As example, when reading the second file, a scale (2,10,4) is applied over every x,y,z
		    contLin ++;


	}
	infile2.close();
	//std::cout <<"contLin="<<contLin<<std::endl;
	MatrixXd B (contLin,3);
	B = readingB.block(0,0,contLin,3);


	std::cout << ">>>>>>>CIERRO INFILE\n"<<std::endl;

	// calcular centroide
	int N=0;
	Matrix3d AA, BB , mCentroidA, mCentroidB;
	MatrixXd H,  U , V ,S;

	std::cout<<"rigid_transform_3D 1"<<std::endl;

	if (A.cols()*A.rows() == B.cols()* B.rows()) { // the 2 arrays have similar size
		MatrixXd AA(A.rows(),A.cols()), BB (B.rows(),B.cols()) , mCentroidA(A.rows(),A.cols()), mCentroidB(B.rows(),B.cols());

		N = A.rows(); // total points

		double centroidAX= VectorXd(A.col(0)).mean();
		double centroidAY= VectorXd(A.col(1)).mean();
		double centroidAZ= VectorXd(A.col(2)).mean();

		Vector3d centroidA(centroidAX,centroidAY,centroidAZ);

		double centroidBX= VectorXd(B.col(0)).mean();
		double centroidBY= VectorXd(B.col(1)).mean();
		double centroidBZ= VectorXd(B.col(2)).mean();

		Vector3d centroidB(centroidBX,centroidBY,centroidBZ);

		for (int i=0; i< N; i++ ) {
			//AA.row(i) = Vector3d(A.row(i)) - centroidA ;
			mCentroidA.row(i) << centroidAX ,centroidAY,centroidAZ ;

		}

		for (int j=0; j< N; j++ ) {
			//BB.row(j) = Vector3d(B.row(j)) - centroidB ;
			mCentroidB.row(j) << centroidBX ,centroidBY,centroidBZ ;

		}
		//AA= A - mCentroidA;
		//BB= B - mCentroidB;

		AA = A.rowwise() - A.colwise().mean();
		BB = B.rowwise() - B.colwise().mean();
		//std::cout << "A \n"<< A <<std:: endl;
		std::cout <<"centroidA" <<std::endl;
		std::cout <<centroidA <<std::endl;
		//std::cout << "AA \n"<< AA << std::endl;

		//std::cout << "B \n"<< B << std::endl;
		std::cout <<"centroidB" <<std::endl;
		std::cout <<centroidB <<std::endl;
		//std::cout << "BB \n"<< BB << std::endl;

		// Hallar el cuadrado de las columnas

        // METHOD 1
		double scalaX = AA.col(0).norm() / BB.col(0).norm();
		double scalaY = AA.col(1).norm() / BB.col(1).norm();
		double scalaZ = AA.col(2).norm() / BB.col(2).norm();

		std::cout <<"scalaX="<<scalaX <<std::endl;
		std::cout <<"scalaY="<<scalaY <<std::endl;
		std::cout <<"scalaZ="<<scalaZ <<std::endl;

		std::cout <<"1/scalaX="<<1/scalaX <<std::endl;
		std::cout <<"1/scalaY="<<1/scalaY <<std::endl;
		std::cout <<"1/scalaZ="<<1/scalaZ <<std::endl;

		scalaX = BB.col(0).norm() / AA.col(0).norm();
		scalaY = BB.col(1).norm() / AA.col(1).norm();
		scalaZ = BB.col(2).norm() / AA.col(2).norm();

		std::cout <<"scalaX="<<scalaX <<std::endl;
		std::cout <<"scalaY="<<scalaY <<std::endl;
		std::cout <<"scalaZ="<<scalaZ <<std::endl;
		std::cout <<"1/scalaX="<<1/scalaX <<std::endl;
		std::cout <<"1/scalaY="<<1/scalaY <<std::endl;
		std::cout <<"1/scalaZ="<<1/scalaZ <<std::endl;

		// How to know, what to divide AA.norm()/ BB.norm() ?
		// or BB.norm() / AA.norm() ?
		//
		// if mean (AA) > mean (BB)
	    //       scale = AA.norm() / BB.norm()
		// else
		//       scale = BB.norm() / AA.norm()


		// METHOD 2, WITH SVD

		AA = A.rowwise() - A.colwise().mean();
		BB = B.rowwise() - B.colwise().mean();
	/*
		//JacobiSVD<MatrixXd> svd(AA,ComputeThinV);
		JacobiSVD<MatrixXd> svd(AA,ComputeFullU | ComputeFullV);
		//MatrixXd W = svd.matrixV().leftCols(3);
		MatrixXd V1 = svd.matrixV();
		std::cout << "V1 \n"<< V1 << std::endl;
		MatrixXd U1 = svd.matrixU();
		//std::cout << "U1 \n"<< U1 << std::endl;
		MatrixXd S1 = svd.singularValues();
		std::cout << "S1 \n"<< S1 << std::endl;
		//MatrixXd W = svd.matrixV().leftCols(3);
		//std::cout << "W \n"<< W << std::endl;
		//MatrixXd R1 = V1*S1;
		MatrixXd R1 = V1.transpose()*S1;
		std::cout << "R1 \n"<< R1 << std::endl;
		//MatrixXd X1 = U1*S1;
		//std::cout << "X1 \n"<< X1 << std::endl;

		JacobiSVD<MatrixXd> svd2(BB,ComputeFullU | ComputeFullV);
		//MatrixXd Z = svd2.matrixV().leftCols(3);
		MatrixXd V2 = svd2.matrixV();
		std::cout << "V2 \n"<< V2 << std::endl;
		MatrixXd U2 = svd2.matrixU();
		//std::cout << "U2 \n"<< U2 << std::endl;
		MatrixXd S2 = svd2.singularValues();
		std::cout << "S2 \n"<< S2 << std::endl;
		//MatrixXd Z = svd2.matrixV().leftCols(3);
		//std::cout << "Z \n"<< Z << std::endl;

		//MatrixXd R2 = V2*S2.transpose();
		MatrixXd R2 = V2.transpose()*S2;
		std::cout << "R2 \n"<< R2 << std::endl;

		std::cout << "SCALE ESTIMATED WITH SDV \n"<<  std::endl;

		scalaX = S2(0)  / S1(0) ;
		scalaY = S2(1)  / S1(1) ;
		scalaZ = S2(2)  / S1(2) ;

		std::cout <<"scalaX="<<scalaX <<std::endl;
		std::cout <<"scalaY="<<scalaY <<std::endl;
		std::cout <<"scalaZ="<<scalaZ <<std::endl;




		/// NUEVO INTENTO
		Eigen::JacobiSVD<Eigen::MatrixXd> svd_1(AA, Eigen::ComputeThinV);

		// and here is the question what is the basis matrix and how can i reduce it
		// in my understanding it should be:
		Eigen::MatrixXd W1 = svd_1.matrixV().leftCols(3);
		std::cout << "W1 \n"<< W1 << std::endl;

		MatrixXd S1_1 = svd.singularValues();
		std::cout << "S1_1 \n"<< S1_1 << std::endl;


		/// method eigen values

		//MatrixXd centered = mat.rowwise() - mat.colwise().mean();
		//MatrixXd cov = centered.adjoint() * centered;
		MatrixXd AAcov = AA.adjoint() * AA;

		//and then perform the eigendecomposition:

		SelfAdjointEigenSolver<MatrixXd> eigA(AAcov);


		//eigenvalues

		std::cout << "eig.eigenvalues() \n"<< eigA.eigenvalues() << std::endl;
		Vector3d eigenValuesA=eigA.eigenvalues() ;

		MatrixXd BBcov = BB.adjoint() * BB;

				//and then perform the eigendecomposition:

		SelfAdjointEigenSolver<MatrixXd> eigB(BBcov);


		//eigenvalues

		std::cout << "eig.eigenvalues() \n"<< eigB.eigenvalues() << std::endl;

		std::cout << "SCALE ESTIMATED WITH EIGEN VALUES \n"<<  std::endl;
        Vector3d eigenValuesB=eigB.eigenvalues() ;
		scalaX = sqrt(eigenValuesB(0)  / eigenValuesA(0)) ;
		scalaY = sqrt(eigenValuesB(1)  / eigenValuesA(1)) ;
		scalaZ = sqrt(eigenValuesB(2)  / eigenValuesA(2)) ;

		std::cout <<"scalaX="<<scalaX <<std::endl;
		std::cout <<"scalaY="<<scalaY <<std::endl;
		std::cout <<"scalaZ="<<scalaZ <<std::endl;
        */
        FindScala myFindScala;
        myFindScala.getScalaSVD(AA,BB);
        myFindScala.getScalaEigenValues(AA,BB);
        myFindScala.getScalaRansac(A,B,contLin);




	}
}
