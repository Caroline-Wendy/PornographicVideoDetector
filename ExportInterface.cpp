/*! @file
********************************************************************************
<PRE>
ģ���� : �����ӿ�
�ļ��� : ExportInterface.cpp
����ļ� : ExportInterface.h
�ļ�ʵ�ֹ��� : �����ӿ���ʵ��
���� : C.L.Wang
Email: morndragon@126.com
�汾 : 1.0
--------------------------------------------------------------------------------
���̰߳�ȫ�� : ��
�쳣ʱ��ȫ�� : ��
--------------------------------------------------------------------------------
��ע : ��
--------------------------------------------------------------------------------
�޸ļ�¼ :  
�� ��              �汾   �޸���         �޸����� 
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <ctime>

#include <opencv.hpp>

#include "ExportInterface.h"
#include "VideoDetector.h"

using namespace cv;
using namespace std;
using namespace vd;

std::ostream& _printLog (std::ostream& os, const std::string& msg);

/*! @function
********************************************************************************
<PRE>
������ : PornVideoSceneNum
���� : ɫ����Ƶ��������
���� : 
const char* const _fileName, �ļ���;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const unsigned int, ��������;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : num = PornVideoSceneNum (_fileName, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const unsigned int PornVideoSceneNum (
	const char* const _fileName,
	const unsigned int _shotInterval, 
	const unsigned int _sceneShotNum
)
{
	std::clog << std::endl;
	_printLog(std::clog, "----------Begin PornVideoSceneNum! ----------");
	std::clog << std::endl;

	int sceneNum (0); //������

	const std::string fileName (_fileName); //��Ƶ�ļ���

	std::cout << " File Name: " << fileName << std::endl;

	cv::VideoCapture iVC (fileName); //��Ƶ
	if(!iVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video! ");
		return 0;
	}

	std::size_t scene_length(_shotInterval*_sceneShotNum); //����֡��

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //�ؼ�֡��

	/*���㳡����*/
	if (framesNum < scene_length) {
		sceneNum = 1;
	} else {
		for (std::size_t j=0; j<framesNum-1; j+=scene_length){
			++sceneNum;
		}
	}

	std::clog << " Frames Number = " << framesNum << std::endl;
	std::clog << " Scenes Number = " << sceneNum << std::endl;

	std::clog << std::endl;
	_printLog(std::clog, "----------End PornVideoSceneNum! ----------");
	std::clog << std::endl;

	return sceneNum;
}

/*! @function
********************************************************************************
<PRE>
������ : PornVideoDetector
���� : ɫ����Ƶ���
���� : 
double* _resultSet, ���صĽ������;
const char* const _fileName, �ļ���;
const char* const _modelPath, ģ��·��;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : 
isOK = PornVideoDetector (
_resultSet, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool PornVideoDetector (
	double* _resultSet,
	const char* const _fileName, 
	const char* const _modelPath,
	const unsigned int _shotInterval, 
	const unsigned int _sceneShotNum
)
{
	std::clog << std::endl;
	_printLog(std::clog, "----------Begin PornVideoDetector! ----------");
	std::clog << std::endl;

	const std::string fileName(_fileName);
	const std::string modelPath(_modelPath);

	const std::size_t shotInterval(_shotInterval);
	const std::size_t sceneInterval(_sceneShotNum*shotInterval);

	std::cout << " File Name: " << fileName << std::endl;

	cv::VideoCapture iVC (fileName); //��Ƶ
	if(!iVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video! ");
		return false;
	}

	const double framesNum (iVC.get(CV_CAP_PROP_FRAME_COUNT)); //�ؼ�֡��
	const double fps (iVC.get(CV_CAP_PROP_FPS)); //ÿ��֡
	double vtime = framesNum / fps;
	std::cout << " Frames Number: " << framesNum << std::endl;
	std::cout << " Video Time: " << vtime << "s " << std::endl;


	std::cout << " Please wait some time! :)" << std::endl;

	std::vector<double> reseltSet;

	VideoDetector iVD(fileName, modelPath, shotInterval, sceneInterval);

	clock_t start=clock();

	if (iVD.detectVideo()) {
		reseltSet = iVD.getResultSet();
	} else {
		return false;
	}

	clock_t end=clock();

	double ptime = static_cast<double>(end-start)/CLOCKS_PER_SEC; //ʱ��

	std::cout << " Processing Time: " << ptime << "s " << std::endl;

	for (std::size_t i=0; i<reseltSet.size(); ++i) {
		_resultSet[i] = reseltSet[i];
		std::cout << " Scene[" << i << "] Result : " << _resultSet[i] << std::endl;
	}

	std::clog << std::endl;
	_printLog(std::clog, "----------End PornVideoDetector! ----------");
	std::clog << std::endl;

	return true;
}

/*! @function
********************************************************************************
<PRE>
������ : PornVideoDetector_Info
���� : ɫ����Ƶ���-��Ϣ
���� : 
SceneInfo* _infoSet, ������Ϣ����;
const char* const _fileName, �ļ���;
const char* const _folderPath, �ļ���·��;
const char* const _modelPath, ģ��·��;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : 
isOK = PornVideoDetector (
_infoSet, _folderPath, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool PornVideoDetector_Info (
	SceneInfo* _infoSet,
	const char* const _folderPath,
	const char* const _fileName, 
	const char* const _modelPath,
	const unsigned int _shotInterval, 
	const unsigned int _sceneShotNum
)
{
	std::clog << std::endl;
	_printLog(std::clog, "----------Begin PornVideoDetector_Image! ----------");
	std::clog << std::endl;

	const std::string fileName(_fileName);
	const std::string modelPath(_modelPath);

	const std::string folderPath(_folderPath);
	const std::string imageName(_fileName);

	const std::size_t shotInterval(_shotInterval);
	const std::size_t sceneInterval(_sceneShotNum*shotInterval);

	std::cout << " File Name: " << fileName << std::endl;

	cv::VideoCapture iVC (fileName); //��Ƶ
	if(!iVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video! ");
		return false;
	}

	const double framesNum (iVC.get(CV_CAP_PROP_FRAME_COUNT)); //�ؼ�֡��
	const double fps (iVC.get(CV_CAP_PROP_FPS)); //ÿ��֡
	double vtime = framesNum / fps;
	std::cout << " Frames Number: " << framesNum << std::endl;
	std::cout << " Video Time: " << vtime << "s " << std::endl;


	std::cout << " Please wait some time! :)" << std::endl;

	std::vector<double> reseltSet;
	std::vector<std::string> nameSet;

	VideoDetector iVD(fileName, modelPath, shotInterval, sceneInterval);

	clock_t start=clock();

	if (iVD.detectVideo()) {
		reseltSet = iVD.getResultSet();
		nameSet = iVD.storeKeyFrames (folderPath, imageName); //�洢�ؼ�֡
	} else {
		return false;
	}

	clock_t end=clock();

	double ptime = static_cast<double>(end-start)/CLOCKS_PER_SEC; //ʱ��

	std::cout << " Processing Time: " << ptime << "s " << std::endl;

	for (std::size_t i=0; i<reseltSet.size(); ++i) {
		_infoSet[i].prop = reseltSet[i];
		strcpy_s(_infoSet[i].imagePath, nameSet[i].c_str());
		//std::cout << " Scene[" << i << "] Result : " << _infoSet[i].prop << std::endl;
		//std::cout << " Scene[" << i << "] Image : " << _infoSet[i].imagePath << std::endl;
	}

	unsigned int sceneNum (0); //������
	if (framesNum < sceneInterval) {
		_infoSet[0].bpos = 0;
		_infoSet[0].epos = static_cast<size_t>(framesNum);
		_infoSet[0].btime = static_cast<double>(_infoSet[0].bpos)/fps;
		_infoSet[0].etime = static_cast<double>(_infoSet[0].epos)/fps;
	} else {
		for (std::size_t j=0; j<framesNum-1; j+=sceneInterval){
			_infoSet[sceneNum].bpos = j;
			if (j < framesNum-1-sceneInterval) {
				_infoSet[sceneNum].epos = j+sceneInterval;
			} else {
				_infoSet[sceneNum].epos =static_cast<std::size_t>(framesNum-1);
			}
			_infoSet[sceneNum].btime = static_cast<double>(_infoSet[sceneNum].bpos)/fps;
			_infoSet[sceneNum].etime = static_cast<double>(_infoSet[sceneNum].epos)/fps;
			++sceneNum;
		}
	}

	std::clog << std::endl;
	_printLog(std::clog, "----------End PornVideoDetector_Image! ----------");
	std::clog << std::endl;

	return true;
}


/*! @function
********************************************************************************
<PRE>
������ : PornVideoDetector_Help
���� : ɫ����Ƶ���-����
���� : void
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : PornVideoDetector_Help();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornVideoDetector_Help (void)
{
	const std::string version = "Version 1.0";
	std::cout << std::endl;
	std::cout << "ɫ����Ƶ��Ƶ�����(" << version << ")" << std::endl;
	std::cout << "����: C.L.Wang" << std::endl;
	std::cout << "Email: morndragon@126.com" << std::endl;
	std::cout << "��Ȩ�����ս���Ȩ��C.L.Wang����" << std::endl;
	std::cout << "�������ֹ�����޸ļ�ʹ��, ����Ҫʹ��, ���ʼ���ϵ." << std::endl;
	std::cout << "�ɰ�Ů��-ľ�����_��." << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "��������: ����ģ��->�����Ƶ�Ƿ�Ϊɫ��." << std::endl;
	std::cout << "������ʹ��, ��Ҫ����ɫ����Ƶ���ģ��." << std::endl;

	std::cout << std::endl << std::endl;

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : _printLog
���� : ��ӡ��Ϣ
���� : 
_os, ��ӡ��ʽ; 
_msg, ��ӡ����;
����ֵ : std::ostream& ��ӡ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : _printLog(cout, "Nothing");
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
std::ostream& _printLog (
	std::ostream& _os, const std::string& _msg)
{
	if (std::cerr == _os) {
		_os << "Warning : " << _msg << std::endl;
	} else if (std::clog == _os) {
		_os << "Log : " << _msg << std::endl;
	} else {
		_os << "Message : " << _msg << std::endl;
	}

	return _os;
}