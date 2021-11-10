#pragma once
namespace Multilayer_Neural_Network
{
		struct Settings {
			typedef enum class _NORMALIZE_WEIGHTS_MODE
			{
				No = 0,
				WiCols_WoRows = 1,
				WiRows_WoCols = 2,
				MatrixMaxMin = 3,
			} NORMALIZE_WEIGHTS_MODE;

			int MaxIteration; //максимальное кол-во итераций
			bool WeightsNormalization; //норамлизация весовых кооэф. 
			int BlockCols;
			int BlockRows;
			int BlockSize;
			int Neurons;
			double AcceptableError;
			double LearningCoefficient;
			NORMALIZE_WEIGHTS_MODE NormalizeWeightsMode = NORMALIZE_WEIGHTS_MODE::WiCols_WoRows;
			//NORMALIZE_WEIGHTS_MODE NormalizeWeightsMode = NORMALIZE_WEIGHTS_MODE::MatrixMaxMin;
		};

		class Block
		{
		private:
			int startCol;
			int startRow;
			MatrixColorFloat vectorX;
		public:
			Block(int startRow, int startCol) {
				this->startCol = startCol;
				this->startRow = startRow;
			};

			//void addElement(double newElement);
			int getStartCol() { return startCol; };
			void setStartCol(int startCol) { this->startCol = startCol; };
			int getStartRow() { return startRow; };
			void setStartRow(int startRow) { this->startRow = startRow; };
			MatrixColorFloat getVectorX() { return vectorX; };
			MatrixColorFloat *getpVectorX() { return &vectorX; };
			void setVectorX(MatrixColorFloat matrix) {
				MatrixColorFloat temp(1, matrix.size());
				int x = 0;
				for (int row = 0; row < matrix.rows(); row++)
					for (int col = 0; col < matrix.cols(); col++)
						temp(0, x++) = matrix(row, col);
				this->vectorX = temp;
			};
		};

	class ANN
	{
		int ChannelCols;
		int ChannelRows;
		int ChannelSize;
		int Channels;
		//так кол-во канналов у GIF могут быть различное, будем хранить вектор из матриц канналов
		std::vector<std::vector<Block>> totalBlokcs;



		//std::vector<std::vector<RectangleModel>> matrixsBlokcs;
		std::vector<MatrixColorFloat> matrixsOfWeightsIn;
		std::vector<MatrixColorFloat> matrixsOfWeightsOut;



	public:
		ANN();
		ANN(int blockrows, int blockcols, int numberofneurons, double acceptableerror, double learningcoefficient, int maxiteration);
		~ANN();
		void initialData(int rows, int cols, int channels, Quantum *data);
		void initialDataGIF(int rows, int cols, int src_channels, int channels, Quantum *data);
		void divideIntoBlocks();
		void training();
		MatrixColor createOutputImageChann(int channel, ofstream &out_file);
		void createAndPushWeightMatrixs();
		void normalizeWeighMatrixs(MatrixColorFloat *w_in, MatrixColorFloat *w_out);

		Settings settings;
		int SizeCompressImage;
		std::vector<MatrixColorFloat> matrixsChannels;
		enum class Error
		{
			OK = 0,
			Open_Image,
		};
		enum class Warning
		{
			OK = 0,
		};
		class State
		{
			Error _error = Error::OK;
			Warning _warning = Warning::OK;
		public:
			Error getError() { return _error; }
			void setError(Error error) { _error = error; }
			Warning getWarning() { return _warning; }
			void setWarning(Warning warning) { _warning = warning; }
		} state;
	};

}