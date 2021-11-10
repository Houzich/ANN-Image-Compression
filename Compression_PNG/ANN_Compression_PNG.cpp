#include "stdafx.h"


//// path to image
//char const *srcImg;


namespace Compression_Plugin
{


	//Entity::Entity(const char* name, float xPos, float yPos)
	//	: m_Name(name), m_XPos(xPos), m_YPos(yPos)
	//{
	//	std::cout << "Created the Entity object!" << std::endl;
	//}
	//void Entity::Move(float deltaX, float deltaY)
	//{
	//	m_XPos += deltaX;
	//	m_YPos += deltaY;
	//	std::cout << "Moved " << m_Name << " to (" << m_XPos << ", " << m_YPos << ")." << std::endl;
	//}

	//int main(int argc, char *argv[]) {
	//	//if (argc != 2) {
	//	//	printf("Not correct argument\n");
	//	//	exit(1);
	//	//}
	//	//srcImg = strdup(argv[1]);
	//	//ImageModel model(srcImg);
	//	//model.run();
	//	//model.createOutputImage();
	//	return 0;
	//}

	ANNCompress::ANNCompress(char *path) {
		Compress(path);
	}

	int ANNCompress::Compress(char *path) {
		ImageModel model(path);
		//model.run();
		//model.createOutputImage();
		return 0;
	}




}