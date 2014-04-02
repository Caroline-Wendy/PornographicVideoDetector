/*! @file
********************************************************************************
<PRE>
模块名 : 导出接口
文件名 : ExportInterface.cpp
相关文件 : ExportInterface.h
文件实现功能 : 导出接口类实现
作者 : C.L.Wang
Email: morndragon@126.com
版本 : 1.0
--------------------------------------------------------------------------------
多线程安全性 : 是
异常时安全性 : 是
--------------------------------------------------------------------------------
备注 : 无
--------------------------------------------------------------------------------
修改记录 :  
日 期              版本   修改人         修改内容 
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

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
函数名 : PornVideoSceneNum
功能 : 色情视频场景数量
参数 : 
const char* const _fileName, 文件名;
const unsigned int _shotInterval, 镜头间隔;
const unsigned int _sceneShotNum, 场景数量;
返回值 : const unsigned int, 场景数量;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : num = PornVideoSceneNum (_fileName, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
作者 : C.L.Wang
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

	int sceneNum (0); //场景数

	const std::string fileName (_fileName); //视频文件名

	std::cout << " File Name: " << fileName << std::endl;

	cv::VideoCapture iVC (fileName); //视频
	if(!iVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video! ");
		return 0;
	}

	std::size_t scene_length(_shotInterval*_sceneShotNum); //场景帧数

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //关键帧数

	/*计算场景数*/
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
函数名 : PornVideoDetector
功能 : 色情视频检测
参数 : 
double* _resultSet, 返回的结果集合;
const char* const _fileName, 文件名;
const char* const _modelPath, 模型路径;
const unsigned int _shotInterval, 镜头间隔;
const unsigned int _sceneShotNum, 场景数量;
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : 
isOK = PornVideoDetector (
_resultSet, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
作者 : C.L.Wang
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

	cv::VideoCapture iVC (fileName); //视频
	if(!iVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video! ");
		return false;
	}

	const double framesNum (iVC.get(CV_CAP_PROP_FRAME_COUNT)); //关键帧数
	const double fps (iVC.get(CV_CAP_PROP_FPS)); //每秒帧
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

	double ptime = static_cast<double>(end-start)/CLOCKS_PER_SEC; //时间

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
函数名 : PornVideoDetector_Info
功能 : 色情视频检测-信息
参数 : 
SceneInfo* _infoSet, 场景信息集合;
const char* const _fileName, 文件名;
const char* const _folderPath, 文件夹路径;
const char* const _modelPath, 模型路径;
const unsigned int _shotInterval, 镜头间隔;
const unsigned int _sceneShotNum, 场景数量;
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : 
isOK = PornVideoDetector (
_infoSet, _folderPath, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
作者 : C.L.Wang
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

	cv::VideoCapture iVC (fileName); //视频
	if(!iVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video! ");
		return false;
	}

	const double framesNum (iVC.get(CV_CAP_PROP_FRAME_COUNT)); //关键帧数
	const double fps (iVC.get(CV_CAP_PROP_FPS)); //每秒帧
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
		nameSet = iVD.storeKeyFrames (folderPath, imageName); //存储关键帧
	} else {
		return false;
	}

	clock_t end=clock();

	double ptime = static_cast<double>(end-start)/CLOCKS_PER_SEC; //时间

	std::cout << " Processing Time: " << ptime << "s " << std::endl;

	for (std::size_t i=0; i<reseltSet.size(); ++i) {
		_infoSet[i].prop = reseltSet[i];
		strcpy_s(_infoSet[i].imagePath, nameSet[i].c_str());
		//std::cout << " Scene[" << i << "] Result : " << _infoSet[i].prop << std::endl;
		//std::cout << " Scene[" << i << "] Image : " << _infoSet[i].imagePath << std::endl;
	}

	unsigned int sceneNum (0); //场景数
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
函数名 : PornVideoDetector_Help
功能 : 色情视频检测-帮助
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : PornVideoDetector_Help();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornVideoDetector_Help (void)
{
	const std::string version = "Version 1.0";
	std::cout << std::endl;
	std::cout << "色情视频视频检测器(" << version << ")" << std::endl;
	std::cout << "作者: C.L.Wang" << std::endl;
	std::cout << "Email: morndragon@126.com" << std::endl;
	std::cout << "版权及最终解释权归C.L.Wang所有" << std::endl;
	std::cout << "本程序禁止擅自修改及使用, 如需要使用, 请邮件联系." << std::endl;
	std::cout << "可爱女孩-木子兮→_→." << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "方法描述: 加载模型->检测视频是否为色情." << std::endl;
	std::cout << "本程序使用, 需要附加色情视频检测模型." << std::endl;

	std::cout << std::endl << std::endl;

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _printLog
功能 : 打印信息
参数 : 
_os, 打印格式; 
_msg, 打印内容;
返回值 : std::ostream& 打印流
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : _printLog(cout, "Nothing");
--------------------------------------------------------------------------------
作者 : C.L.Wang
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