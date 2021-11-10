#include "stdafx.h"


using namespace Multilayer_Neural_Network;
namespace Compression_Plugin
{


	ImageModel::ImageModel()
	{
	}

	ImageModel::~ImageModel()
	{
	}
	//читаем изображение в матрицы
	void ImageModel::raedImageToMatrix(char const * patch, int blockrows, int blockcols) {
		try {
			try
			{
				image.read(patch);
				image.crop(Geometry((image.columns() / blockcols)*blockcols, (image.rows() / blockrows)*blockrows, 0, 0));
			}
			catch (Magick::WarningCoder &warning)
			{
				ERROR_READ_IMAGE_TO_ARRAY("IMAGE OPEN ERROR|Magick exception| Coder Warning: " << warning.what());
				state.setError(Error::Open_Image);
				return;
			}
			catch (Magick::Warning &warning)
			{
				// Handle any other Magick++ warning.
				ERROR_READ_IMAGE_TO_ARRAY("IMAGE OPEN ERROR|Magick exception| Warning: " << warning.what());
				state.setError(Error::Open_Image);
				return;
			}
			catch (Magick::ErrorFileOpen &error)
			{
				// Process Magick++ file open error
				ERROR_READ_IMAGE_TO_ARRAY("IMAGE OPEN ERROR|Magick exception| Error: " << error.what());
				state.setError(Error::Open_Image);
				return;
			}
		}
		catch (std::exception & error)
		{
			ERROR_READ_IMAGE_TO_ARRAY("IMAGE OPEN ERROR|STD exception| Error: " << error.what());
			state.setError(Error::Open_Image);
			return;
		}
		catch (...)
		{
			ERROR_READ_IMAGE_TO_ARRAY("IMAGE OPEN ERROR|Unknown exception| Error");
			state.setError(Error::Open_Image);
			return;
		}
		//Открыли файл
		imageWidth = (int)image.columns();
		imageHeight = (int)image.rows();
		imageNumChannel = (int)image.channels();
		imageFormat = image.magick();
		pixelMatrixCols = imageWidth;
		pixelMatrixRows = imageHeight;
		imagePath = patch;
		imagePixels = image.getPixels(0, 0, imageWidth, imageHeight);

#ifdef IMAGE_GRAYSCALE		
		image.grayscale(MagickCore::PixelIntensityMethod::BrightnessPixelIntensityMethod);
		string path = patch;
		Image image_greyscale = image;
		path.insert(path.rfind("."), "_source");
		image_greyscale.write(path);
		imageNumChannel = (int)image.channels();
		imagePixels = image.getPixels(0, 0, imageWidth, imageHeight);
		//imageNumChannel = 1;
#else
#endif // IMAGE_GRAYSCALE

		DEBUG_READ_IMAGE_TO_ARRAY("Image width: " << imageWidth);
		DEBUG_READ_IMAGE_TO_ARRAY("Image height: " << imageHeight);
		DEBUG_READ_IMAGE_TO_ARRAY("Image channels: " << imageNumChannel);
	}

	void ImageModel::init(char const * patch, int blockrows, int blockcols, int numberofneurons, double acceptableerror, double learningcoefficient, int maxiteration) {
		double err = acceptableerror;
		double coeff = learningcoefficient;
		//читаем изображение
		raedImageToMatrix(patch, blockrows, blockcols); 
		if (state.getError() != Error::OK) { return; }

		err = acceptableerror / CORRECT_ERR;
		coeff = learningcoefficient / CORRECT_COEFF;
		//инициализируем нейронную сеть
		ANN Compress(blockrows, blockcols, numberofneurons, err, coeff, maxiteration);

		if (imageFormat == "GIF") {
#ifdef IMAGE_GRAYSCALE		
			Compress.initialDataGIF(imageHeight, imageWidth, 2, 1, imagePixels);
			imageFormat = "GRAYSCALE_GIF";
#else
			Compress.initialDataGIF(imageHeight, imageWidth, 5, 1, imagePixels);
#endif // IMAGE_GRAYSCALE
		}
		else {
			Compress.initialData(imageHeight, imageWidth, imageNumChannel, imagePixels);
		}
		//делим изображение на блоки и инициалзируем матрицы весов
		Compress.divideIntoBlocks();
		//обучаем нейронную сеть
		Compress.training();
		if (imageFormat == "GIF") {
			createOutputImageGIF(&Compress, 5, 1);
		}
		else if (imageFormat == "GRAYSCALE_GIF") {
			createOutputImageGIF(&Compress, 2, 1);
		}
		else {
			createOutputImage(&Compress);
		}
	}




	void ImageModel::createOutputImage(ANN *compress) {
		string path = "";
		//Для каждого каннала 
		//MatrixPxls Test_MatrixOut;
		std::vector<MatrixColor> arrayImagesChannelCompress;

		std::ofstream out_file("Images\\Сompressed.bin", std::ios::binary | std::ios::out);
		for (int chn = 0; chn < imageNumChannel; chn++) {
			arrayImagesChannelCompress.push_back(compress->createOutputImageChann(chn, out_file));
		}
		out_file.close(); // закрываем файл

		for (int chn = 0; chn < imageNumChannel; chn++) {
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("SOURCE");
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("Channel[" << chn << "]:\n" << compress->matrixsChannels[chn]);
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("COMPRESS");
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("ChannelCompres[" << chn << "]:\n" << arrayImagesChannelCompress[chn]);
		}

		DEBUG("Cols: " << arrayImagesChannelCompress[0].cols());
		DEBUG("Rows: " << arrayImagesChannelCompress[0].rows());
		Magick::Image image_out = image;
		Quantum *pixels_out = image_out.getPixels(0, 0, pixelMatrixCols, pixelMatrixRows);
		Quantum *pixels = pixels_out;
				for (int row_idx = 0; row_idx < pixelMatrixRows; row_idx++)
					for (int col_idx = 0; col_idx < pixelMatrixCols; col_idx++)
					{
						for (int chn = 0; chn < imageNumChannel; chn++) {
							*pixels++ = arrayImagesChannelCompress[chn](row_idx, col_idx);
						}
					}


		image_out.syncPixels();
		path = imagePath;
		path.insert(path.rfind("."), "_decompressed");
		//image_out.compressType(MagickCore::CompressionType::LZWCompression);
		image_out.write(path);
	}



	void ImageModel::createOutputImageGIF(ANN *compress, int src_channels, int channels) {
		string path = "";
		//Для каждого каннала 
		//MatrixPxls Test_MatrixOut;
		std::vector<MatrixColor> arrayImagesChannelCompress;

		std::ofstream out_file("Images\\Сompressed.bin", std::ios::binary | std::ios::out);
		for (int chn = 0; chn < imageNumChannel; chn++) {
			arrayImagesChannelCompress.push_back(compress->createOutputImageChann(chn, out_file));
		}
		out_file.close(); // закрываем файл

		for (int chn = 0; chn < channels; chn++) {
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("SOURCE");
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("Channel[" << chn << "]:\n" << compress->matrixsChannels[chn]);
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("COMPRESS");
			DEEP_DEBUG_CREATE_OUTPUT_IMAGE("ChannelCompres[" << chn << "]:\n" << arrayImagesChannelCompress[chn]/ QuantumRange);
		}

		DEBUG("Cols: " << arrayImagesChannelCompress[0].cols());
		DEBUG("Rows: " << arrayImagesChannelCompress[0].rows());
		Magick::Image image_out = image;
		Quantum *pixels_out = image_out.getPixels(0, 0, pixelMatrixCols, pixelMatrixRows);
		Quantum *pixels = pixels_out;
		int offset = src_channels - channels;
		int i = offset;
#ifdef IMAGE_GRAYSCALE
		 offset = src_channels - channels;
		 i = 0;
#endif // IMAGE_GRAYSCALE
		for (int row_idx = 0; row_idx < pixelMatrixRows; row_idx++)
			for (int col_idx = 0; col_idx < pixelMatrixCols; col_idx++)
			{
				for (int chn = 0; chn < channels; chn++)
				{
					pixels[i++] = arrayImagesChannelCompress[chn](row_idx, col_idx);
					if (chn == (channels - 1)) { i += offset; }
				}
			}

		image_out.syncPixels();
		path = imagePath;
		// уменьшили энтропию изображения теперь сжимаем кодированием
		image_out.compressType(MagickCore::CompressionType::LZWCompression);
		path.insert(path.rfind("."), "_compress");
		image_out.write(path);
	}
}
