/*! @file
********************************************************************************
<PRE>
模块名 : 关键帧
文件名 : MiddleKeyFrames.cpp
相关文件 : MiddleKeyFrames.h
文件实现功能 : 中间关键帧类实现
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
2014/03/27    1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "MiddleKeyFrames.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : MiddleKeyFrames
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : MiddleKeyFrames iMKF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
MiddleKeyFrames::MiddleKeyFrames (void) : 
	KeyFrames()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : MiddleKeyFrames
功能 : 参数构造函数
参数 : 
const string& _videoName, 视频名; 
const size_t  _shotInterval, 镜头间隔;
const size_t _beg, 起始位置; 
const size_t _end, 终止位置;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : MiddleKeyFrames iMKF(videoName, shotInterval, beg, end);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
MiddleKeyFrames::MiddleKeyFrames (
	const std::string& _videoName, 
	const std::size_t  _shotInterval, 
	const std::size_t _beg, 
	const std::size_t _end
	) :
	KeyFrames(_videoName, _shotInterval, _beg, _end)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~MiddleKeyFrames
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iMKF.~MiddleKeyFrames();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
MiddleKeyFrames::~MiddleKeyFrames (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : extractKeyFrames
功能 : 提取关键帧
参数 : void
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : isOK = extractKeyFrames();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool MiddleKeyFrames::extractKeyFrames (void)
{
	cv::VideoCapture iVC(m_videoName); //视频类

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //帧数

	if (m_end > framesNum) { //越界则设置为末尾
		m_end = framesNum; 
	}

	const std::size_t sfNum = m_end - m_beg; //场景长度 

	if (framesNum < (m_shotInterval/2+1)) { //镜头过长
		__printLog(std::cerr, "The shot interval is too long, please reset.");
		return false;
	}

	/*缩放*/

	const double scale = iVC.get(CV_CAP_PROP_FRAME_HEIGHT)/FIXED_HIGHT;
	int srcRows = static_cast<int>(
		iVC.get(CV_CAP_PROP_FRAME_WIDTH)/scale);
	int srcCols = static_cast<int>(
		iVC.get(CV_CAP_PROP_FRAME_HEIGHT)/scale);
	cv::Size srcSize(srcRows, srcCols);

	/*提取关键集合*/

	for (std::size_t i=m_beg + (m_shotInterval/2); i<(m_beg+sfNum-1); i+=m_shotInterval)
	{
		cv::Mat frameTemp; //需要每次都新建frame

		iVC.set(CV_CAP_PROP_POS_FRAMES, i);

		iVC >> frameTemp;

		if (frameTemp.empty())
			break;

		cv::resize(frameTemp, frameTemp, srcSize);
		m_pKeyFrames->emplace_back(frameTemp);

		m_pFramePositions->push_back(i);

		frameTemp.release();
	}

	m_isKeyFrames = true;

	return true;
}