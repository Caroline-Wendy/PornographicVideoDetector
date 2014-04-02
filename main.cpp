#include "stdafx.h"

#include <iostream>

#include "ExportInterface.h"

using namespace std;

void test (void)
{
	PornVideoDetector_Help();

	const std::string fileName("TestVideo.avi");
	const std::string modelPath("./models");
	const unsigned int shotInterval(100);
	const unsigned int sceneShotNum(20);

	unsigned int num = 
		PornVideoSceneNum(fileName.c_str(), shotInterval, sceneShotNum);

	double* resultSet = new double[num];

	if (PornVideoDetector(resultSet, fileName.c_str(), modelPath.c_str(), 
			shotInterval, sceneShotNum)) 
	{
		for (unsigned int i=0; i<num; ++i) {
			std::cout << " Scene[" << i << "] Result : " << resultSet[i] << std::endl;
		}
	} else {
		std::cout << " Failed to detect the video! " << std::endl;
	}

	delete[] resultSet;
	resultSet = nullptr;

	return;
}

void test_image (void) 
{
	const std::string fileName("Oh!.mp4");
	const std::string imagePath("./images");
	const std::string modelPath("./models");
	const unsigned int shotInterval(100);
	const unsigned int sceneShotNum(20);

	unsigned int num = PornVideoSceneNum(fileName.c_str(), 
		shotInterval, sceneShotNum);

	SceneInfo* resultSet = new SceneInfo[num];

	if (PornVideoDetector_Info(resultSet, imagePath.c_str(), fileName.c_str(), 
		modelPath.c_str(), shotInterval, sceneShotNum)) 
	{
		for (unsigned int i=0; i<num; ++i) {
			std::cout << " Scene[" << i << "] Prop : " << resultSet[i].prop << std::endl;
			std::cout << " Scene[" << i << "] Image Path : " 
				<< resultSet[i].imagePath << std::endl;
			std::cout << " Scene[" << i << "] Begin Time : " << resultSet[i].btime << std::endl;
			std::cout << " Scene[" << i << "] End Time : " << resultSet[i].etime << std::endl;
			std::cout << " Scene[" << i << "] Begin Pos : " << resultSet[i].bpos << std::endl;
			std::cout << " Scene[" << i << "] End Pos : " << resultSet[i].epos << std::endl;
		}
	} else {
		std::cout << " Failed to detect the video! " << std::endl;
	}

	delete[] resultSet;
	resultSet = nullptr;

	PornVideoDetector_Help();

}


int main (void)
{
	test();

	return 0;
}