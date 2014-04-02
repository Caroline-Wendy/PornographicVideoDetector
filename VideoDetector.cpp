/*! @file
********************************************************************************
<PRE>
模块名 : 检测视频
文件名 : VideoDetector.cpp
相关文件 : VideoDetector.h
文件实现功能 : 视频检测器实现
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
日期                版本   修改人         修改内容 
2014/03/26   1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "VideoDetector.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : VideoDetector
功能 : 参数构造函数
参数 : 
const string& _fileName, 文件名;
 const string& _modelPath, 模型路径; 
const size_t _shotInterval, 镜头间隔;
const size_t _sceneInterval, 场景间隔;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : VideoDetector(_fileName, _modelPath, _shotInterval, _sceneInterval);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
VideoDetector::VideoDetector (
	const std::string& _fileName,
	const std::string& _modelPath,
	const std::size_t _shotInterval,
	const std::size_t _sceneInterval
) : 
	m_fileName(_fileName),
	m_modelPath(_modelPath),
	m_shotInterval(_shotInterval),
	m_sceneInterval(_sceneInterval)
{
	std::size_t maxShotInterval(999); 
	std::size_t maxSceneInterval(9999);

	/*判断文件是否存在*/

	std::ifstream ifile;
	ifile.open(m_fileName, ios::in);
	if (ifile.fail()) {
		_printLog(std::cerr, "Failed to open the video!");
		throw std::exception("File Name Error");
	}
	ifile.close();

	/*镜头间隔判断*/

	if (m_shotInterval < 1 || m_shotInterval > maxShotInterval) {
		_printLog(std::cerr, "The value of the shot interval is invalid.");
		throw std::exception("Shot Interval Error");
	}

	/*场景间隔判断*/

	if (m_sceneInterval < 1 || m_sceneInterval > maxSceneInterval) {
		_printLog(std::cerr, "The value of the scene interval is invalid.");
		throw std::exception("Scene Interval Error");
	}

	/*判断文件夹是否存在*/

	if (_access(m_modelPath.c_str(), 0)) {
		_printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~VideoDetector
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iVD.~VideoDetector();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
VideoDetector::~VideoDetector(void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : detectVideo
功能 : 检测视频
参数 : void
返回值 : const bool, 是否成功
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : isOK = iVD.detectVideo();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool VideoDetector::detectVideo (void)
{
	/*判断视频是否可读*/

	cv::VideoCapture iVC (m_fileName);

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //总帧数

	SceneIntegration iSI(m_fileName, m_modelPath, m_shotInterval, m_sceneInterval);

	double finalLabel (0.0); //存储视频结果
	std::size_t beg (0);
	std::size_t sceneNum(0);

	for (std::size_t j=0; j<framesNum; j+=m_sceneInterval)
	{
		/*提取关键帧集合*/

		finalLabel = iSI.predictSceneProp(beg);

		std::cout << " Scene Prop = " << finalLabel << std::endl;

		/*存储结果*/
		m_resultSet.push_back(finalLabel);

		beg += m_sceneInterval;

		++sceneNum;

	}

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : getResultSet
功能 : 返回结果集合
参数 : void
返回值 : const vector<double>, 结果集合
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : vd = iVD.getResultSet();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::vector<double> VideoDetector::getResultSet(void) const
{
	if (m_resultSet.empty())
		throw exception("Result Set Empty");
	return m_resultSet;
}

/*! @function
********************************************************************************
<PRE>
函数名 : getSceneNum
功能 : 返回场景数量
参数 : void
返回值 : const size_t, 场景数量
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : num = iVD.getSceneNum();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::size_t VideoDetector::getSceneNum (void)
{
	std::size_t sceneNum (0); //场景数

	cv::VideoCapture iVC (m_fileName);

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //关键帧数

	if (framesNum < m_sceneInterval) {
		sceneNum = 1;
	} else {
		for (std::size_t j=0; j<framesNum; j+=m_sceneInterval) {
			++sceneNum;
		}
	}

	return sceneNum;
}

/*! @function
********************************************************************************
<PRE>
函数名 : storeKeyFrames
功能 : 存储关键帧
参数 : 
const std::string& _folderPath, 文件夹路径;
const std::string& _imageName, 图像名称;
返回值 : const vector<string>, 图片位置集合
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : vs = iVD.storeKeyFrames(_folderPath, _imageName);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::vector<std::string> VideoDetector::storeKeyFrames (
	const std::string& _folderPath, const std::string& _imageName)
{
	std::vector<std::string> names; //返回名称

	cv::VideoCapture cVC (m_fileName);

	const std::size_t framesNum(
		static_cast<std::size_t>(cVC.get(CV_CAP_PROP_FRAME_COUNT))); //总帧数

	std::size_t beg (0);
	std::size_t sceneNum(0);
	std::vector<cv::Mat> keyFrames;
	std::vector<std::size_t> framePositions;

	for (std::size_t i = 0; i<framesNum-1; i+=m_sceneInterval)
	{
		MiddleKeyFrames iMKF(m_fileName, m_shotInterval, beg, beg+m_sceneInterval);

		if(!iMKF.extractKeyFrames() ) {
			_printLog(std::cerr, "Failed to extract key frames.");
			return names;
		}

		keyFrames = *iMKF.getKeyFrames();
		framePositions = *iMKF.getFramePositions();

		std::string name;

		std::string ul("_");
		std::stringstream ss;
		ss << framePositions[framePositions.size()/2];
		std::string digit = ss.str();
		std::string jpg(".jpg");

		name.append(_folderPath);
		name.append("/");
		name.append(_imageName);
		name.append(ul);
		name.append(digit);
		name.append(jpg);

		if(keyFrames[keyFrames.size()/2].data) {
			cv::imwrite(name, keyFrames[keyFrames.size()/2]);
			names.push_back(name);
		} 

		beg += m_sceneInterval;

		++sceneNum;
	}
	
	return names;
}

/*! @function
********************************************************************************
<PRE>
函数名 : help
功能 : 帮助信息
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iVD.help();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void VideoDetector::help (void)
{
	const std::string version = "Version 1.0";
	std::cout << std::endl;
	std::cout << "色情视频视频检测器(" << version << ")" << std::endl;
	std::cout << "作者: C.L.Wang" << std::endl;
	std::cout << "Email: morndragon@126.com" << std::endl;
	std::cout << "版权及最终解释权归C.L.Wang所有" << std::endl;
	std::cout << "本程序禁止擅自修改及使用, 如需修改或使用, 请邮件联系." << std::endl;
	std::cout << "可爱女孩-木子兮→_→." << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "方法描述: 加载模型->检测视频." << std::endl;
	std::cout << "本程序使用, 需要附加色情视频检测模型." << std::endl;

	std::cout << std::endl << std::endl;

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __printLog
功能 : 打印信息
参数 : 
_os, 打印格式; 
_msg, 打印内容;
返回值 : std::ostream& 打印流
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
std::ostream& VideoDetector::_printLog (
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