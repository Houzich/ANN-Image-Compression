#include "stdafx.h"
namespace Multilayer_Neural_Network
{
	using Mathematical_Operations::MathOperations;

	ANN::ANN()
	{
		settings.BlockRows = 8;
		settings.BlockCols = 8;
		settings.BlockSize = settings.BlockRows * settings.BlockCols;
		settings.Neurons = 48;
		settings.AcceptableError = 0.1 * settings.Neurons;
		settings.LearningCoefficient = 0.0005;
	}


	ANN::ANN(int blockrows, int blockcols, int numberofneurons, double acceptableerror, double learningcoefficient, int maxiteration)
	{
		settings.WeightsNormalization = false;
		settings.BlockRows = blockrows;
		settings.BlockCols = blockcols;
		settings.BlockSize = settings.BlockRows * settings.BlockCols;
		settings.Neurons = numberofneurons;
		settings.AcceptableError = acceptableerror;
		settings.LearningCoefficient = learningcoefficient;
		settings.MaxIteration = maxiteration;
	}

	ANN::~ANN()
	{
	}

	void ANN::initialData(int rows, int cols, int channels, Quantum *data)
	{
		ChannelCols = cols;
		ChannelRows = rows;
		ChannelSize = ChannelCols * ChannelRows;
		Channels = channels;

		//заполняем вектор канналов пустыми массивами соответствующие размерам изображения
		MatrixColorFloat chann_matrix(rows, cols);
		for (int i = 0; i < channels; i++) {
			matrixsChannels.push_back(chann_matrix);
		}

		//заполняем массивы каналов цвета
		for (int row_idx = 0; row_idx < rows; row_idx++)
			for (int col_idx = 0; col_idx < cols; col_idx++)
			{
				for (int chn = 0; chn < channels; chn++) {
					matrixsChannels[chn](row_idx, col_idx) = *data++;
				}
			}

		for (int chn = 0; chn < channels; chn++)
			DEEP_DEBUG_DATA_TO_ARRAY("Channel[" << chn << "]:\n" << matrixsChannels[chn]);
	}
	//заполняем матрицы и устанавливаем начальные значения для дальнейшей работы ANN
	//data - это буфер содержащий данные изображения
	void ANN::initialDataGIF(int rows, int cols, int src_channels, int channels, Quantum *data) {

		ChannelCols = cols;
		ChannelRows = rows;
		ChannelSize = ChannelCols * ChannelRows;
		Channels = channels;

		//заполняем вектор канналов пустыми массивами соответствующие размерам изображения
		MatrixColorFloat chann_matrix(rows, cols);
		for (int i = 0; i < channels; i++) {
			matrixsChannels.push_back(chann_matrix);
		}

		//заполняем массивы каналов цвета
		int offset = src_channels - channels;
		int i = offset;
#ifdef IMAGE_GRAYSCALE
		 offset = src_channels - channels;
		 i = 0;
#endif // IMAGE_GRAYSCALE
		for (int row_idx = 0; row_idx < rows; row_idx++)
			for (int col_idx = 0; col_idx < cols; col_idx++)
			{
				for (int chn = 0; chn < channels; chn++) {
					matrixsChannels[chn](row_idx, col_idx) = data[i++];
					if (chn == (channels - 1)) { i += offset; }
				}
			}

		for (int chn = 0; chn < channels; chn++)
			DEEP_DEBUG_DATA_TO_ARRAY("Channel[" << chn << "]:\n" << matrixsChannels[chn]);
	}

	//делим изображение на блоки и инициализируем/заполняем случайными числами матрицы весов
	void ANN::divideIntoBlocks()
	{
		/*imageType = image.type();*/
		//Magick::ColorGray  clr;
		//Для каждого каннала 
		for (int chn = 0; chn < Channels; chn++) {
			MatrixColorFloat *mtx_img_chn = &matrixsChannels[chn];
			int cnt_windows = 0;
			std::vector<Block> channel_blocks;
			//Делим изображение на блоки (окна)
			for (int col_idx = 0; col_idx < ChannelCols; col_idx += settings.BlockCols) {
				for (int row_idx = 0; row_idx < ChannelRows; row_idx += settings.BlockRows) {
					Block block(row_idx, col_idx);

					MatrixColorFloat mtx_wind = mtx_img_chn->block(row_idx, col_idx, settings.BlockRows, settings.BlockCols);
					//Если блоки выходят за границы изображения (Лучше подбирать так блоки, чтоб были кратны размеру изображения)
					//то дублируем соответствующие крайние строки и столбцы (или можно туда 0 значения вставить)
					int brink_cols = (col_idx + settings.BlockCols) - ChannelCols; //кол-во колонок выходящих за границы изображения
					int brink_rows = (row_idx + settings.BlockRows) - ChannelRows; //кол-во строк выходящих за границы изображения
					if ((brink_cols > 0) || (brink_rows > 0))
					{
						MathOperations::fillBrinkBlock(&mtx_wind, brink_rows, brink_cols);
					}
					//переводим значения цветов в диапазон [-1.0,1.0]
					MathOperations::convertColorMatrixToInputANN(&mtx_wind);
					//значения цветов пикселей из блока собираем в свой(эталонный) вектор X для этоко окна.
					block.setVectorX(mtx_wind);
					cnt_windows++;
					channel_blocks.push_back(block);
				}
			}
			totalBlokcs.push_back(channel_blocks);
			createAndPushWeightMatrixs();
		}

	}


	void ANN::training() {
		double coeff_in;
		double coeff_out;
		double E;
		int L = (int)totalBlokcs[0].size(); //кол-во блоков(окон)
		int iteration = 0;
		MESSAGE_ANN_LEARNING(cout << "ANN Learning" << endl);
		MESSAGE_ANN_LEARNING(cout << "Block: " << settings.BlockCols <<"x" << settings.BlockRows 
								  << ", Neurons: " << settings.Neurons 
								  << ", Acceptable Error: " << settings.AcceptableError*CORRECT_ERR
								  << endl);

		double z = (L * settings.BlockSize * Channels) / ((L* settings.Neurons)*Channels + settings.Neurons*settings.BlockSize*2);
		MESSAGE_ANN_LEARNING(cout << "Compression ratio Z: " << z << endl);
		MESSAGE_ANN_LEARNING(cout << "Blokcs: " << L << endl);
		//Для каждого каннала 
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		for (int chn = 0; chn < Channels; chn++) {
			MatrixColorFloat *Win = &matrixsOfWeightsIn[chn]; //матрица весов прямого слоя 
			MatrixColorFloat *Wout = &matrixsOfWeightsOut[chn]; //матрица весов обратного слоя 
			MatrixColorFloat *Xin;	//временный вектор исходных данных для каждого блока
			MatrixColorFloat Y;	//временный вектор сжатых данных для каждого блока
			MatrixColorFloat Xout;	//временный вектор восстановленных данных для каждого блока
			MatrixColorFloat deltaX;

			//нормализуем матрицы
			if(settings.WeightsNormalization)normalizeWeighMatrixs(Win, Wout);
			iteration = 0; //считаем количество итераций (обучений нейронов)
			int init_list_deltaX = 0;
			std::vector<MatrixColorFloat*> list_deltaX;
			do {
				for (int index = 0; index < L; index++) {
					//MESSAGE_ANN_LEARNING(cout << "Curr Blokc :" << index << endl);
					Xin = totalBlokcs[chn][index].getpVectorX();
					Y = (*Xin) * (*Win);
					Xout = Y * (*Wout);
					deltaX = Xout - (*Xin);  //обратная связь между входом X и выходом Y
#ifdef DEBUG_SPEED
					list_deltaX.push_back(&deltaX);
#endif // DEBUG_SPEED
				
					//if ((Xin->cols() != Xout.cols()) || (Xin->rows() != Xout.rows())) {
					//	cout << "ERROR Xin and Xout" << endl;
					//	return;
					//}	

					DEEP_DEBUG_ANN_LEARNING("X :\n" << *Xin);
					DEEP_DEBUG_ANN_LEARNING("Win :\n" << *Win);
					DEEP_DEBUG_ANN_LEARNING("Y :\n" << Y);
					DEEP_DEBUG_ANN_LEARNING("Wout :\n" << *Wout);
					DEEP_DEBUG_ANN_LEARNING("X_ :\n " << Xout);
					DEEP_DEBUG_ANN_LEARNING("deltaX :\n" << deltaX);

					if (settings.LearningCoefficient) {
						coeff_out = coeff_in = settings.LearningCoefficient;
					}
					else {
						coeff_in = MathOperations::adaptiveLearningCoefficient(Xin);
						coeff_out = MathOperations::adaptiveLearningCoefficient(&Y);
					}

					if (index == L - 1) { break; }

					*Win = *Win - coeff_in * (Xin->transpose() * deltaX * Wout->transpose()); //обучаем прямой слой
					*Wout = *Wout - (coeff_out * Y.transpose() * deltaX); //обучаем обратный слой
					if (settings.WeightsNormalization)normalizeWeighMatrixs(Win, Wout);
					DEEP_DEBUG_ANN_LEARNING("Win :\n" << Win);
					DEEP_DEBUG_ANN_LEARNING("Wout :\n" << Wout);
				}

				E = 0; // cуммарная среднеквадратическая ошибка
#ifdef DEBUG_SPEED
				//После обучения определяем суммарную среднеквадратическую ошибку
				for (int index = 0; index < L; index++) {
					E += MathOperations::getErrorDegree(list_deltaX[index]);
				}
#else
				E = settings.AcceptableError + 1;
#endif // DEBUG_SPEED
				iteration++;
				if(iteration%MESSAGE_LEARNING_INTERVAL ==0)
					MESSAGE_ANN_LEARNING(cout << "Iteration: " << iteration << ", coeff_in: " << coeff_in* CORRECT_COEFF << ", coeff_out: " << coeff_out * CORRECT_COEFF << ", Total SSE: " << E * CORRECT_ERR << "\r");
				//повторяем алгоритм пока суммарная ошибка не станет меньше приемлемой
			} while (E > settings.AcceptableError && !_kbhit()&& iteration<settings.MaxIteration);
			MESSAGE_ANN_LEARNING(cout << "Iteration: " << iteration << ", coeff_in: " << coeff_in * CORRECT_COEFF << ", coeff_out: " << coeff_out * CORRECT_COEFF << ", Total SSE: " << E * CORRECT_ERR << "\r");

			DEBUG_ANN_LEARNING("Channel " << chn << ":");
			DEBUG_ANN_LEARNING("Total SSE = " << E * CORRECT_ERR << ", Iterations = " << iteration);
			//сохраняем весовый матрицы для восстановления.
			matrixsOfWeightsIn[chn] = *Win;
			matrixsOfWeightsOut[chn] = *Wout;
		}
	}



	

	//Test.push_back('1');
	//Test.push_back('2');
	//Test.push_back('3');
	//Test.push_back('4');

	//
	//out_file.write((const char*)&Test.front(), Test.size());
	//Test.push_back('5');
	//Test.push_back('6');
	//out_file.write((const char*)&Test.front(), Test.size());
	//out_file.close(); // закрываем файл
	//std::ofstream out_file("Images\\Compression.bin", std::ios::binary | std::ios::out);


	MatrixColor ANN::createOutputImageChann(int channel, ofstream &out_file) {
		MatrixColor result(ChannelRows, ChannelCols);
		vector<uint8_t> VectorToFile;
		int L = (int)totalBlokcs[channel].size(); //кол-во блоков(окон)
		MatrixColorFloat W = matrixsOfWeightsIn[channel];
		MatrixColorFloat W_ = matrixsOfWeightsOut[channel];
		VectorToFile = MathOperations::matrixToByteVector(W_);
		out_file.write((const char*)&VectorToFile.front(), VectorToFile.size());

		for (int index = 0; index < L; index++) {
			int startCol = totalBlokcs[channel][index].getStartCol();
			int startRow = totalBlokcs[channel][index].getStartRow();
			MatrixColorFloat X = totalBlokcs[channel][index].getVectorX();
			MatrixColorFloat Y = X * W;
			MatrixColorFloat X_ = Y * W_;
			VectorToFile = MathOperations::matrixToByteVector(Y);
			out_file.write((const char*)&VectorToFile.front(), VectorToFile.size());
			//MatrixColorFloat Y = X_ * W;
			//MatrixColorFloat X_ = Y * W_;
			DEEP_DEBUG_DECOMPRESS_CHANN("X: " << endl << X << endl);
			DEEP_DEBUG_DECOMPRESS_CHANN("X_: " << endl << X << endl);
			int idx_pxl_block = 0;
			for (int row_idx = startRow; row_idx < settings.BlockRows + startRow; row_idx++) {
				for (int col_idx = startCol; col_idx < settings.BlockCols + startCol; col_idx++) {
					if (col_idx < ChannelCols && row_idx < ChannelRows) {
						Quantum  clr = MathOperations::convertPixelToOutput(X_(idx_pxl_block++, 0));
						result(row_idx, col_idx) = clr;
						//result(row_idx, col_idx) = X(idx_pxl_block++, 0);
						DEEP_DEBUG_DECOMPRESS_CHANN("col: " << col_idx << ", row: " << row_idx);
						DEEP_DEBUG_DECOMPRESS_CHANN("Input: " << X(idx_pxl_block, 0) << ", idx_pxl_block: " << idx_pxl_block);
						DEEP_DEBUG_DECOMPRESS_CHANN("Output: " << X_(idx_pxl_block, 0) << ", idx_pxl_block: " << idx_pxl_block);						 //DEEP_DEBUG_READ_IMAGE_TO_ARRAY("Color: " << clr);
					}
					else {
						idx_pxl_block++;
					}
				}
			}
		}
		return result;
	}

	void ANN::createAndPushWeightMatrixs() {
		//Непрерывное равномерное распределение в диапазоне [-1.0,1.0]
		MatrixColorFloat w_matrix = MatrixColorFloat::Random(settings.BlockSize, settings.Neurons);
#ifdef COLOR_FLOAT_RANGE_0_1
		//Непрерывное равномерное распределение в диапазоне [0.0,1.0]
		w_matrix = (w_matrix.array() + 1.0) / 2.0;
		//w_matrix.array() = 0.3;
#endif // COLOR_FLOAT_RANGE_0_1
		//w_matrix.array() = 0.3;
		MatrixColorFloat w_matrix_trans = w_matrix.transpose();
		matrixsOfWeightsIn.push_back(w_matrix);
		matrixsOfWeightsOut.push_back(w_matrix_trans);
		DEBUG_CREATE_WEIGHT_MATRIX("Matrix W:\n" << w_matrix);
		DEBUG_CREATE_WEIGHT_MATRIX("Matrix W_:\n" << w_matrix_trans);
	}

	void ANN::normalizeWeighMatrixs(MatrixColorFloat *w_in, MatrixColorFloat *w_out) {
		if (settings.NormalizeWeightsMode == Settings::NORMALIZE_WEIGHTS_MODE::WiCols_WoRows)
		{
			MathOperations::normalizeColsMatrix(w_in);
			MathOperations::normalizeRowsMatrix(w_out);
		}
		else if (settings.NormalizeWeightsMode == Settings::NORMALIZE_WEIGHTS_MODE::WiRows_WoCols)
		{
			MathOperations::normalizeRowsMatrix(w_out);
			MathOperations::normalizeColsMatrix(w_in);
		}
		else if (settings.NormalizeWeightsMode == Settings::NORMALIZE_WEIGHTS_MODE::MatrixMaxMin)
		{
			MathOperations::normalizeMinMaxMatrix(w_in);
			MathOperations::normalizeMinMaxMatrix(w_out);
		}
		else 
		{

		}
	}
}
