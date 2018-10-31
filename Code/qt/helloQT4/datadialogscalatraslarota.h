#ifndef DATADIALOGSCALATRASLAROTA_H
#define DATADIALOGSCALATRASLAROTA_H
#include <string>
//class QString;
class DataDialogScalaTraslaRota{
private:

double scaleX;
double scaleY;
double scaleZ;

double traslaX;

double traslaY;

double traslaZ;

double rotaX;

double rotaY;

double rotaZ;

double gNoise;
double cNoise;
double timeOffset;
int pcaIndex;

public:
DataDialogScalaTraslaRota();
DataDialogScalaTraslaRota(double sX,double sY, double sZ,double tX,double tY, double tZ, double rX, double rY, double rZ,double gNoise, double cNoise ,double tO,int pca);
void updateData(double sX,double sY, double sZ,double tX,double tY, double tZ, double rX, double rY, double rZ,double gN , double cN, double tO,int pca);
//DataDialogScalaTraslaRota(char* sX, char* sY, char* sZ,char* tX,char* tY, char* tZ, char* rX, char* rY, char* rZ);
double getScaleX();
double getScaleY();
double getScaleZ();
double getTraslaX();
double getTraslaY();
double getTraslaZ();
double getRotaX();
double getRotaY();
double getRotaZ();
double getGaussianNoiseDeviation();
double getCosmicNoiseDeviation();
double getTimeOffset();
int getPcaIndex();

void setScaleX(double aValue);
void setScaleY(double aValue);
void setScaleZ(double aValue);
void setTraslaX(double aValue);
void setTraslaY(double aValue);
void setTraslaZ(double aValue);
void setRotaX(double aValue);
void setRotaY(double aValue);
void setRotaZ(double aValue);
void setGaussianNoiseDeviation(double aValue);
void setCosmicNoiseDeviation(double aValue);
void setTimeOffset(double aValue);
void setPcaIndex(int aValue);

};
#endif // DATADIALOGSCALATRASLAROTA_H
