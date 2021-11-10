#pragma once
namespace Compression_Plugin
{
	//namespace Cpp
	//{
	//	//// This is our native implementation
	//	//// It's marked with __declspec(dllexport) 
	//	//// to be visible from outside the DLL boundaries
	//	//class __declspec(dllexport) Logic
	//	//{
	//	//public:
	//	//	int Get() const; // That's where our code goes
	//	//};
	//}

	//class Entity
	//{
	//public:
	//	const char* m_Name;
	//private:
	//	float m_XPos, m_YPos;
	//public:
	//	Entity(const char* name, float xPos, float yPos);

	//	void Move(float deltaX, float deltaY);
	//	inline float GetXPosition() const { return m_XPos; };
	//	inline float GetYPosition() const { return m_YPos; };
	//};
   
	class ANNCompress
	{
		public:
			ANNCompress(char *path);
			int Compress(char *path);
	};
	

}