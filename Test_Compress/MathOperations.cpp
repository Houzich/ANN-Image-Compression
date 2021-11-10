#include "stdafx.h"

namespace Mathematical_Operations
{

	MathOperations::MathOperations()
	{
	}


	MathOperations::~MathOperations()
	{
	}


	//матрица окна
	//brink_cols - кол-во колонок выходящих за границы изображения
	//brink_rows - кол-во строк выходящих за границы изображения
	void MathOperations::fillBrinkBlock(MatrixColorFloat *mtx, int brink_rows, int brink_cols) {
		int mtx_cols = (int)mtx->cols();
		int mtx_rows = (int)mtx->rows();
		if (brink_cols >= mtx_cols || brink_rows >= mtx_rows) {
			//Ошибка, выходим. 
			cerr << "ERROR fillBrinkWindowMatrix()!" << endl;
			return;
		}

		//Заполняем колонки
		if (brink_cols > 0)
		{
			int idx = mtx_cols - brink_cols; //индекс первой колонки выходящей за границы изображения
			for (int i = idx; i < mtx_cols; i++)
			{
				//для того чтоб не запутаться с функцией .block() введем переменные
				int startRow = 0;
				//int startCol =
				int numRows = mtx_rows;
				int numCols = 1;
				mtx->block(startRow, i, numRows, numCols) = mtx->block(startRow, (idx - 1), numRows, numCols);
			}
		}
		//Заполняем строки
		if (brink_rows > 0)
		{
			int idx = mtx_rows - brink_rows; //индекс первой строки выходящей за границы изображения
			for (int i = idx; i < mtx_rows; i++)
			{
				//для того чтоб не запутаться с функцией .block() введем переменные
				//int startRow = 0;
				int startCol = 0;
				int numRows = 1;
				int numCols = mtx_cols;
				mtx->block(i, startCol, numRows, numCols) = mtx->block((idx - 1), startCol, numRows, numCols);
			}
		}
	}

	//нормализуем матрицу по колонкам (переводим в диапазон 0.0 - 1.0)
	void MathOperations::normalizeColsMatrix(MatrixColorFloat *matrix) {
		if ((matrix->maxCoeff() > 1.0) || (matrix->minCoeff() < 0.0)) {
			for (int col_idx = 0; col_idx < matrix->cols(); col_idx++)
				matrix->col(col_idx) = matrix->col(col_idx).normalized();
		}
	}
	//нормализуем матрицу по строкам (переводим в диапазон 0.0 - 1.0)
	void MathOperations::normalizeRowsMatrix(MatrixColorFloat *matrix) {
		if ((matrix->maxCoeff() > 1.0) || (matrix->minCoeff() < 0.0)) {
			for (int row_idx = 0; row_idx < matrix->rows(); row_idx++)
				matrix->row(row_idx) = matrix->row(row_idx).normalized();
		}
	}

	void MathOperations::normalizeMinMaxMatrix(MatrixColorFloat *matrix) {
		if ((matrix->maxCoeff() > 1.0) || (matrix->minCoeff() < 0.0)) {
			*matrix = matrix->array() + abs(matrix->minCoeff());
			*matrix /= matrix->maxCoeff();
		}
	}


	double MathOperations::adaptiveLearningCoefficient(MatrixColorFloat *matrix) {
		double FACTOR = 100.0;
		return (1.0 / (matrix->squaredNorm()+FACTOR));
	}

	//конвентирует матрицу канала цвета в диапазон для алгоритма ANN
	MatrixColorFloat MathOperations::convertColorMatrixToInputANN(MatrixColorFloat mtx) {
#ifdef COLOR_FLOAT_RANGE_0_1
		return mtx = (mtx * 1.0) / QuantumRange;
#else
		mtx = (mtx * 2) / QuantumRange;
		return mtx = mtx.array() - 1;
#endif	 // COLOR_FLOAT_RANGE_0_1	

	}
	//конвентирует матрицу канала цвета в диапазон для алгоритма ANN
	void MathOperations::convertColorMatrixToInputANN(MatrixColorFloat *mtx) {
#ifdef COLOR_FLOAT_RANGE_0_1
		*mtx = (*mtx * 1.0) / QuantumRange;
#else
		*mtx = (*mtx * 2) / QuantumRange;
		*mtx = mtx->array() - 1;
#endif	 // COLOR_FLOAT_RANGE_0_1	
	}

	MagickCore::Quantum MathOperations::convertPixelToOutput(double color) {
		double cl_f;
#ifdef COLOR_FLOAT_RANGE_0_1
		cl_f = (double)QuantumRange * color;
#else
		cl_f = (double)QuantumRange * (color + 1) / 2;
#endif	 // COLOR_FLOAT_RANGE_0_1			
		if (cl_f < 0.0) {
			cl_f = 0.0;
		}
		if (cl_f >(double)QuantumRange) {
			cl_f = (double)QuantumRange;
		}
		Quantum cl = (Quantum)cl_f;
		return (Quantum)cl;
	}

	uint8_t MathOperations::convertPixelToByte(double color) {
		double cl_f;
#ifdef COLOR_FLOAT_RANGE_0_1
		cl_f = 255.0 * color;
#else
		cl_f = 255.0 * (color + 1) / 2;
#endif	 // COLOR_FLOAT_RANGE_0_1			
		if (cl_f < 0.0) {
			cl_f = 0.0;
		}
		if (cl_f >255.0) {
			cl_f = 255.0;
		}
		uint8_t cl = (uint8_t)round(cl_f);
		return cl;
	}


	double MathOperations::getErrorDegree(MatrixColorFloat *deltaX) {
		double e = deltaX->squaredNorm();
		if (e < 0) {
			DEBUG("e IS NAN");
		}
		if (std::isnan(e))
		{
			DEBUG("e IS NAN");
			e = 99999.0;
		}
		if (std::isinf(e))
		{
			DEBUG("e IS INF");
			e = 99999.0;
		}
		return e;
	}

	vector<uint8_t> MathOperations::matrixToByteVector(MatrixColorFloat X) {
		vector<uint8_t> v;
		for (int row_idx = 0; row_idx < X.rows(); row_idx++)
			for (int col_idx = 0; col_idx < X.cols(); col_idx++)
			{
				v.push_back(MathOperations::convertPixelToByte(X(row_idx, col_idx)));
			}
		return v;
	}



}