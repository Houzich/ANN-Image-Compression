#pragma once
#include "stdafx.h"

namespace Compression_Plugin
{
	using Multilayer_Neural_Network::ANN;


	class ImageModel
	{
		Magick::Image image;
		int imageWidth;
		int imageHeight;
		int imageNumChannel;
		string imageFormat;
		int pixelMatrixCols;
		int pixelMatrixRows;

		Quantum *imagePixels;
		std::string imagePath;

		MatrixColorFloat Test_Matrix;

	public:
		ImageModel();
		~ImageModel();
		void init(char const * patch, int blockrows, int blockcols, int numberofneurons, double acceptableerror, double learningcoefficient, int maxiteration);
		void createOutputImage(ANN *compress);
		void createOutputImageGIF(ANN *compress, int src_channels, int channels);
		void raedImageToMatrix(char const * patch, int blockrows, int blockcols);
		void SaveMatrixInFile(string str, MatrixColorFloat &matrix);

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
