#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <iostream>
//#include <CImg.h>
//#include <vector>
//#include <armadillo>

#include "rectanglemodel.h"

using namespace std;
//using namespace arma;

class ImageModel
{
private:
    int RGB = 3;
    int imageWidth;
    int imageHeight;
	int imageType;
    int n;
    int m;
    int p;
    double e;
    int L;
    int nmRGB;
    double a;
    vector<RectangleModel> rectangleModelList;
    //mat W;
    //mat W_;
	ublas::matrix<double> W;
	ublas::matrix<double> W_;

    int convertRGBToOutput(double rgb);
    double getErrorDegree(ublas::matrix<double> deltaX);
    void createWeightMatrix();
    double convertColor(int color);
    void normalizeMatrix(ublas::matrix<double> matrix);
    void normalizeMatrixs();
    double adaptiveLearningStep(ublas::matrix<double> matrix);
public:
    ImageModel(char const * patch);
    void run();
    void initANN();
    void createOutputImage();
};

#endif // IMAGEMODEL_H
