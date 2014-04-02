/*! @file
********************************************************************************
<PRE>
模块名 : 关键帧
文件名 : KeyFrames.cpp
相关文件 : KeyFrames.h
文件实现功能 : 关键帧类实现
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

#include "KeyFrames.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : KeyFrames
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : KeyFrames iKF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
KeyFrames::KeyFrames (void) : 
	m_isVideo(false), /*未加载图像*/
	m_isKeyFrames(false), /*未提取特征*/
	m_pKeyFrames(new std::vector<cv::Mat>), /*关键帧指针*/
	m_pFramePositions(new std::vector<std::size_t>) /*帧位置指针*/
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : KeyFrames
功能 : 参数构造函数
参数 : 
const std::string& _videoName, 视频名; 
const std::size_t  _shotInterval, 镜头间隔; 
const std::size_t _beg, 起始位置; 
const std::size_t _end, 终止位置;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : KeyFrames iKF(_videoName);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
KeyFrames::KeyFrames (
	const std::string& _videoName, 
	const std::size_t  _shotInterval, 
	const std::size_t _beg, 
	const std::size_t _end
) :
	m_videoName(_videoName), /*视频*/
	m_shotInterval(_shotInterval), /*镜头间隔*/
	m_beg(_beg), //开始
	m_end(_end), //结束
	m_isKeyFrames(false), /*未提取特征*/
	m_pKeyFrames(new std::vector<cv::Mat>), /*特征向量指针*/
	m_pFramePositions(new std::vector<std::size_t>) /*帧位置指针*/
{
	cv::VideoCapture iVC(m_videoName);

	if (!iVC.isOpened()) { //测试视频
		__printLog(std::cerr, "Failed to open the video in <KeyFrames>!");
		throw std::exception("Video Error");
	}

	m_isVideo = true; //视频可以打开

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~KeyFrames
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iKF.~KeyFrames();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
KeyFrames::~KeyFrames (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : loadVideo
功能 : 加载视频
参数 : 
const std::string& _videoName, 视频名称;
const std::size_t  _shotInterval, 镜头间隔;
const std::size_t _beg, 起始位置;
const std::size_t _end, 终止位置;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iKF.loadVideo(videoName);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void KeyFrames::loadVideo (
	const std::string& _videoName, 
	const std::size_t  _shotInterval, 
	const std::size_t _beg, 
	const std::size_t _end
)
{
	__resetData(); //重置数据

	m_videoName = _videoName;
	m_shotInterval = _shotInterval; /*镜头间隔*/
	m_beg = _beg; //开始
	m_end = _end; //结束

	cv::VideoCapture iVC(m_videoName);

	if (!iVC.isOpened()) { //测试视频
		__printLog(std::cerr, "Failed to open the video in <KeyFrames>!");
	}


	m_isVideo = true;

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : storeKeyFrames
功能 : 存储关键帧
参数 : 
const std::string& _folderPath, 文件夹路径; 
const std::string& _imageName, 图像名称;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iKF.storeKeyFrames(folderPath, imageName);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void KeyFrames::storeKeyFrames (
	const std::string& _folderPath, 
	const std::string& _imageName) const
{
	if (_access(_folderPath.c_str(), 0)) { //测试文件夹
		__printLog(std::cerr, "Failed to open the folder! ");
		return;
	}

	if (!m_isKeyFrames) { //测试关键帧
		__printLog(std::cerr, "Please extract key frames first! ");
		return;
	}

	std::size_t kfNum = m_pKeyFrames->size(); //关键帧数

	if (0 == kfNum) {
		__printLog(std::clog, "No key frames! ");
		return;
	}

	for (std::size_t i=0; i<kfNum; ++i) 
	{
		std::string name; //图像名

		std::string ul("_");
		std::stringstream ss;
		ss << i;
		std::string digit = ss.str();
		std::string jpg(".jpg");

		name.append(_folderPath);
		name.append("/");
		name.append(_imageName);
		name.append(ul);
		name.append(digit);
		name.append(jpg);

		if((*m_pKeyFrames)[i].data) {
			cv::imwrite(name, (*m_pKeyFrames)[i]);
		}
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : getKeyFrames
功能 : 返回关键帧
参数 : void
返回值 : const shared_ptr<vector<Mat> > 关键帧
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : pKeyFrames = iKF.getKeyFrames();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::shared_ptr<std::vector<cv::Mat> > 
	KeyFrames::getKeyFrames(void) const
{
	if (m_isKeyFrames) {
		return m_pKeyFrames;
	} else {
		__printLog(std::cerr, "Please extract key frames first!");
		return nullptr;
	}
}

/*! @function
********************************************************************************
<PRE>
函数名 : getFramePositions
功能 : 返回帧位置
参数 : void
返回值 : const shared_ptr<vector<size_t> > 帧位置
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : pFramePositions = iKF.getFramePositions();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::shared_ptr<std::vector<std::size_t> > 
	KeyFrames::getFramePositions(void) const
{
	if (m_isKeyFrames) {
		return m_pFramePositions;
	} else {
		__printLog(std::cerr, "Please extract key frames first!");
		return nullptr;
	}
}

/*! @function
********************************************************************************
<PRE>
函数名 : __resetData
功能 : 重置数据
参数 : void
返回值 : void 
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __resetData();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void KeyFrames::__resetData (void)
{
	m_videoName.clear();

	m_isVideo = false;

	m_isKeyFrames = false;

	if (m_pKeyFrames != nullptr) {
		m_pKeyFrames->clear();
		m_pKeyFrames.reset(new std::vector<cv::Mat>);
	}

	if (m_pFramePositions != nullptr) {
		m_pFramePositions->clear();
		m_pFramePositions.reset(new std::vector<std::size_t>);
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
std::ostream& KeyFrames::__printLog (
	std::ostream& _os, const std::string& _msg) const
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