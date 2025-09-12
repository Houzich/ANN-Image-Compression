// Test_Compress.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace Compression_Plugin;

int main()
{
	std::cout << "Start!" << std::endl;
	ANNCompress::Compress((char *)"Images\\test_PNG.png");
    return 0;
}

namespace Compression_Plugin
{
	ANNCompress::ANNCompress(char *path) {
		Compress(path);
	}

	using Mathematical_Operations::MathOperations;

	int ANNCompress::Compress(char *path) {
		InitializeMagick(NULL);
		ImageModel model;

		vector<uint8_t> Test;
		MatrixColorFloat matrix(2,5);
		matrix << '1', '2', '3', '4', '5',
				 '6', '7', '8', '9', 'A';
		matrix = matrix/255;
		matrix *= QuantumRange;
		DEBUG(matrix);
		MathOperations::convertColorMatrixToInputANN(&matrix);
		DEBUG(matrix);
		Test = MathOperations::matrixToByteVector(matrix);

		std::ofstream out_file("Images\\vector.bin", std::ios::binary | std::ios::out);
		out_file.write((const char*)&Test.front(), Test.size());

		out_file.close(); // закрываем файл

		//// создаЄм объект класса ofstream дл€ записи и св€зываем его с файлом 
		//std::ofstream fout("copy_test.bin", std::ofstream::binary);

		//fout << Test;
		//for (auto i : contents) {
		//	int value = i;
		//	std::cout << "data: " << value << std::endl;
		//}

		//fout.close(); // закрываем файл







			//blockrows - высота блока
			//blockcols - ширина блока
			//numberofneurons - кол-во нейронов
			//acceptableerror - допустима€ ошибка
			//learningcoefficient - коэффициент вли€ющий на шаг обучени€
			//0 <acceptableerror < 0.1*numberofneurons Ц допустима€ ошибка,
			//0 <learningcoefficient < 0.01 Ц коэффициент обучени€, 
			//если 0 то подбираетс€ автоматически с адаптированием.
		model.init(path, 2, 2, 1, 100, 0, 1000);
		return 0;
	}
}

