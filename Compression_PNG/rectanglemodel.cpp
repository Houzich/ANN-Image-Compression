#include "stdafx.h"

using namespace boost::numeric::ublas;


RectangleModel::RectangleModel(int startX, int startY){
    this->startX = startX;
    this->startY = startY;
}

void RectangleModel::createMatrixX(){
	X = trans(matrix<double>(vectorX[0], vectorX[1]));
}

void RectangleModel::addElement(double newElement){
	vectorX.insert_element(0, newElement);
}

int RectangleModel::getStartX(){
    return startX;
}

void RectangleModel::setStartX(int startX){
    this->startX = startX;
}

int RectangleModel::getStartY(){
    return startY;
}

void RectangleModel::setStartY(int startY){
    this->startY = startY;
}

ublas::vector<double> RectangleModel::getVectorX(){
    return vectorX;
}

void RectangleModel::setVectorX(ublas::vector<double> vectorX){
    this->vectorX = vectorX;
	//this->vectorX.resize(vectorX.size());
	//std::copy(vectorX.begin(), vectorX.end(), this->vectorX.begin());
}

ublas::matrix<double> RectangleModel::getX(){
    return X;
}

void RectangleModel::setX(ublas::matrix<double> X){
    this->X = X;
}

