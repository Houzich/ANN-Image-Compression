#pragma once
namespace Mathematical_Operations
{
	class MathOperations
	{
	public:
		MathOperations();
		~MathOperations();


		static void normalizeColsMatrix(MatrixColorFloat *matrix);
		static void normalizeRowsMatrix(MatrixColorFloat *matrix);
		static void normalizeMinMaxMatrix(MatrixColorFloat *matrix);
		static double adaptiveLearningCoefficient(MatrixColorFloat *matrix);
		static void fillBrinkBlock(MatrixColorFloat *mtx, int brink_rows, int brink_cols);
		static MatrixColorFloat convertColorMatrixToInputANN(MatrixColorFloat mtx);
		static void convertColorMatrixToInputANN(MatrixColorFloat *mtx);
		static MagickCore::Quantum convertPixelToOutput(double color);
		static uint8_t convertPixelToByte(double color);
		static double getErrorDegree(MatrixColorFloat *deltaX);
		static vector<uint8_t> matrixToByteVector(MatrixColorFloat X);
	};

}