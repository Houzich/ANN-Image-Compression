#ifndef RECTANGLEMODEL_H
#define RECTANGLEMODEL_H

//#include <armadillo>
//#include <vector>

//using namespace std;
//using namespace arma;


#include <boost/numeric/ublas/matrix.hpp>
#include "boost/numeric/ublas/vector.hpp"
namespace ublas = boost::numeric::ublas;



class RectangleModel
{
private:
    int startX;
    int startY;
	ublas::vector<double> vectorX;
    //mat X;
	ublas::matrix<double> X;
public:
    RectangleModel(int startX, int startY);
    void createMatrixX();
    void addElement(double newElement);
    int getStartX();
    void setStartX(int start);
    int getStartY();
    void setStartY(int start);
	ublas::vector<double> getVectorX();
    void setVectorX(ublas::vector<double> vector);
    //mat getX();
    //void setX(mat x);
	ublas::matrix<double> getX();
	void setX(ublas::matrix<double> x);
};

#endif // RECTANGLEMODEL_H
