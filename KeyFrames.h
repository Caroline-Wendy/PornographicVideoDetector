/*! @file
********************************************************************************
<PRE>
模块名 : 关键帧
文件名 : KeyFrames.h 
相关文件 : KeyFrames.cpp
文件实现功能 : 关键帧类声明
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

#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <cstddef>
#include <io.h>
#include <climits>

#include <opencv.hpp>

//视频检测命名空间
namespace vd 
{
	/*! @class
	********************************************************************************
	<PRE>
	类名称 : KeyFrames
	功能 : 关键帧
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : KeyFrames iKF
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class KeyFrames 
	{
	public:

		KeyFrames (void); //构造函数

		explicit KeyFrames (
			const std::string& _videoName, 
			const std::size_t  _shotInterval = 200, 
			const std::size_t _beg = 0, 
			const std::size_t _end = UINT_MAX
		); //参数构造函数

		virtual ~KeyFrames(); //析构函数

	public:

		void loadVideo (
			const std::string& _videoName, 
			const std::size_t  _shotInterval = 200, 
			const std::size_t _beg = 0, 
			const std::size_t _end = UINT_MAX
			); //载入视频

		virtual const bool extractKeyFrames (void) = 0; //提取关键帧

		void storeKeyFrames (const std::string& _folderPath, const std::string& _imageName) const; //存储关键帧

		const std::shared_ptr<std::vector<cv::Mat> > getKeyFrames (void) const; //返回关键帧

		const std::shared_ptr<std::vector<std::size_t> > getFramePositions (void) const; //返回帧位置

	protected:

		std::string m_videoName; //视频名称

		std::size_t  m_shotInterval; //镜头间隔

		std::size_t m_beg; //开始

		std::size_t m_end; //结束

		bool m_isVideo; //是否可读视频

		bool m_isKeyFrames; //是否提取关键帧

		std::shared_ptr<std::vector<cv::Mat> > m_pKeyFrames; //关键帧集合

		std::shared_ptr<std::vector<std::size_t> > m_pFramePositions; //帧位置

	protected:

		inline void __resetData (void); //重置数据

		inline std::ostream& __printLog (
			std::ostream& os, const std::string& msg) const; //打印信息

	};
}