#include "stdafx.h"

#include <iostream>


//#include <vector>
//#include <armadillo>
#include <cmath>
#include <ctime>
#include <float.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>


#include "imagemodel.h"
#include "rectanglemodel.h"

using namespace std;
using namespace cv;
//using namespace arma;

ImageModel::ImageModel(char const * patch){

	Mat image;
	image = imread(patch, 1);
	if (!image.data)
	{
		printf(" No image data \n ");
		return;
	}

	//Mat gray_image;
	//cvtColor(image, gray_image, CV_BGR2GRAY);

    initANN();
    imageWidth = image.cols;
    imageHeight = image.rows;
	imageType = image.type();
    for (int indexW = 0; indexW < imageWidth; indexW +=m){
        for (int indexH = 0; indexH < imageHeight; indexH +=n){
            RectangleModel bufferRectangle(indexW,indexH);
            for (int i = indexW; i < indexW + m; i++){
                for (int j = indexH; j < indexH + n; j++) {
                    if (i < imageWidth && j < imageHeight){
                        bufferRectangle.addElement(convertColor((int)image.at<Vec3b>(i, j)[0]));
                        bufferRectangle.addElement(convertColor((int)image.at<Vec3b>(i, j)[1]));
                        bufferRectangle.addElement(convertColor((int)image.at<Vec3b>(i, j)[2]));
                    } else {
                        bufferRectangle.addElement(-1);
                        bufferRectangle.addElement(-1);
                        bufferRectangle.addElement(-1);
                    }
                }
            }
            bufferRectangle.createMatrixX();
            rectangleModelList.push_back(bufferRectangle);
        }
    }
    L = rectangleModelList.size();
    nmRGB = n * m * RGB;
    createWeightMatrix();
}

void ImageModel::initANN(){
    cout << "Enter hight of rectangle(n):" << endl;
    cin >> n;
    cout << "Enter width of rectangle(m):" << endl;
    cin >> m;
    cout << "Enter number of neuron for second layer(p):" << endl;
    cin >> p;
    cout << "Enter error degree(e):" << endl;
    cin >> e;
    cout << "Enter step(a) (Enter 0 for adaptive learnin step):" << endl;
    cin >> a;
}

void ImageModel::run(){
    double step;
    double step_;
    double E;
    //normalizeMatrixs(); // uncomment if necessary normalization
    int iteration = 0;
    do {
        E = 0;
        for (int index = 0; index < L; index++){
			ublas::matrix<double> X = rectangleModelList[index].getX();
			ublas::matrix<double> Y = element_prod(X , W);
			ublas::matrix<double> X_ = element_prod(Y , W_);
			ublas::matrix<double> deltaX = X_ - X;
            if (a){
                step_ = step = a;
            } else {
                step_ = adaptiveLearningStep(Y);
                step = adaptiveLearningStep(X);
            }
            W = W - (step * element_prod(element_prod(trans(X) , deltaX) , trans(W_)));
            W_ = W_ - (step_ * element_prod(trans(Y) , deltaX));
            //normalizeMatrixs(); // uncomment if necessary normalization
        }
        // count error after correction
        for (int index = 0; index < L; index++){
			ublas::matrix<double> X = rectangleModelList[index].getX();
			ublas::matrix<double> Y = element_prod(X , W);
			ublas::matrix<double> X_ = element_prod(Y , W_);
			ublas::matrix<double> deltaX = X_ - X;
            E += getErrorDegree(deltaX);
        }
        iteration++;
        cout << "Iteration: " << iteration << " Error: " << E << endl;
    } while (E > e);

    double z = (1.0 * n * m * RGB * L) / ((n * m * RGB + L) * p + 2);
    cout << "Z = " << z << endl;;
}

void ImageModel::normalizeMatrixs(){
    normalizeMatrix(W);
    normalizeMatrix(W_);
}

void ImageModel::normalizeMatrix(ublas::matrix<double> matrix){
    for (unsigned int i = 0; i < matrix.size1(); i++) {
        double sum = 0;
        for (unsigned int j = 0; j < matrix.size2(); j++) {
            sum += pow(matrix(j, i), 2);
        }
        sum = sqrt(sum);
        for (unsigned int j = 0; j < matrix.size2(); j++) {
            matrix(j, i) = matrix(j, i) / sum;
        }
    }
}

double ImageModel::adaptiveLearningStep(ublas::matrix<double> matrix){
    int FACTOR = 10;
	ublas::matrix<double> temp = element_prod(matrix , trans(matrix));
    return 1.0 / (temp(0,0) + FACTOR);
}

void ImageModel::createOutputImage(){
	Mat image = Mat(imageWidth, imageHeight, imageType);
	Vec3b color;
    for (int index = 0; index < L; index++){
        int startX = rectangleModelList[index].getStartX();
        int startY = rectangleModelList[index].getStartY();
		ublas::matrix<double> X = rectangleModelList[index].getX();
		ublas::matrix<double> Y = element_prod(X , W);
		ublas::matrix<double> X_ = element_prod(Y , W_);
        int pixel = 0;
        for (int i = startX; i < m + startX; i++) {
            for (int j = startY; j < n + startY; j++) {
                color[0] = convertRGBToOutput(X_(0, pixel++));
                color[1] = convertRGBToOutput(X_(0, pixel++));
                color[2] = convertRGBToOutput(X_(0, pixel++));
                if (i < imageWidth && j < imageHeight){
                    //image.draw_point(i,j,color);
					image.at<Vec3b>(i, j) = color;
					//image.at<Vec3b>(i, j)[0] = color[0];
					//image.at<Vec3b>(i, j)[1] = color[1];
					//image.at<Vec3b>(i, j)[2] = color[2];
                }
            }
        }
    }
	imwrite("output.png", image);
}

int ImageModel::convertRGBToOutput(double color){
    double ans = (255 * (color + 1) / 2);
    if (ans < 0){
        ans = 0;
    }
    if (ans > 255){
        ans = 255;
    }
    return (int)ans;
}

double ImageModel::getErrorDegree(ublas::matrix<double> deltaX){
    double e=0;
    for (int i = 0; i < nmRGB; i++) {
        e += pow(deltaX(0, i), 2);
    }
    return e;
}

void ImageModel::createWeightMatrix(){
    srand (time(NULL));
    W = ublas::matrix<double>(nmRGB,p);
    for (int i = 0; i < nmRGB; i++){
        for (int j = 0; j < p; j++)
            W(i,j) = (((double)rand() / RAND_MAX)*2 - 1 )*0.1;;
    }
    W_ = trans(W);
}

double ImageModel::convertColor(int color){
    return ((2.0 * color / 255) - 1);
}

