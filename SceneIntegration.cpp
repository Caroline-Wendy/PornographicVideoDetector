/*! @file
********************************************************************************
<PRE>
模块名 : 场景集成
文件名 : SceneIntegration.cpp
相关文件 : SceneIntegration.h
文件实现功能 : 场景集成类实现
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
2014/03/28    1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "SceneIntegration.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : SceneIntegration
功能 : 参数构造函数
参数 : 
const string& _fileName, 视频名;
const size_t  _shotInterval, 镜头间隔;
const size_t _sceneInterval, 场景间隔;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : SceneIntegration iSI(fileName, shotInterval, sceneInterval);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
SceneIntegration::SceneIntegration (
	const std::string& _fileName, 
	const std::string& _modelPath,
	const std::size_t _shotInterval,
	const std::size_t _sceneInterval
	) : 
	m_fileName(_fileName),
	m_modelPath(_modelPath),
	m_shotInterval(_shotInterval),
	m_sceneInterval(_sceneInterval),
	m_pKeyFrames(new std::vector<cv::Mat>),
	m_framePositions(new std::vector<std::size_t>)
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

	/*判断文件是否能打开*/

	if (_access(m_modelPath.c_str(), 0)) {
		_printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

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

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~SceneIntegration
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iSI.~SceneIntegration ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
SceneIntegration::~SceneIntegration (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : predictSceneProp
功能 : 预测场景概率
参数 : const size_t _beg, 场景起始位置
返回值 : const double, 场景概率
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : value = iSI.predictSceneProp (_beg);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double SceneIntegration::predictSceneProp (const std::size_t _beg) //预测场景值
{
	double finalResult(0.0);
	std::vector<double> resultSet;

	cv::VideoCapture cVC (m_fileName); //音频读取

	if (!cVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video!");
		return -1.0;
	}

	std::size_t framesNum = 
		static_cast<std::size_t>(cVC.get(CV_CAP_PROP_FRAME_COUNT)); //总帧数

	MiddleKeyFrames iMKF(m_fileName, m_shotInterval, _beg, _beg+m_sceneInterval);

	if(!iMKF.extractKeyFrames() ) {
		_printLog(std::cerr, "Failed to extract key frames.");
		return 1.0;
	}

	m_pKeyFrames = iMKF.getKeyFrames();
	m_framePositions = iMKF.getFramePositions();

	double value(0.0); //临时值
	PornFeatures iCF(m_modelPath);
	iCF.loadModels();

	for (size_t i=0; i != m_pKeyFrames->size(); ++i)
	{
		iCF.loadImage((*m_pKeyFrames)[i]);
		if (iCF.extractFeatures())
			value = iCF.predictPornImage();

		resultSet.push_back(value);
	}

	double total = std::accumulate(resultSet.begin(), resultSet.end(), 0.0);
	finalResult = total/static_cast<double>(resultSet.size());

	return finalResult;
}


/*! @function
********************************************************************************
函数名 : getKeyInformation
功能 : 获取关键帧信息
参数 : void
返回值 : 
shared_ptr<vector<Mat> >& _keyFrames, 关键帧集合;
vector<size_t>& _framePositions, 关键帧位置集合;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : getKeyFrames();
--------------------------------------------------------------------------------
作者 : C.L.Wang
*******************************************************************************/ 
void SceneIntegration::getKeyInformation (
	std::vector<cv::Mat>& _keyFrames,
	std::vector<std::size_t>& _framePositions
)
{
	if (m_pKeyFrames != nullptr || m_framePositions != nullptr) {
		_keyFrames = *m_pKeyFrames;
		_framePositions = *m_framePositions;
	}

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
std::ostream& SceneIntegration::_printLog (
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