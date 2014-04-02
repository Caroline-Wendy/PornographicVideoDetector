/*! @file
********************************************************************************
<PRE>
模块名 : 视频检测
文件名 : VideoDetector.h 
相关文件 : VideoDetector.cpp
文件实现功能 : 视频检测器类声明
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
2014/03/25    1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>

#include "SceneIntegration.h"
#include "MiddleKeyFrames.h"

//视频检测命名空间
namespace vd 
{
		/*! @class
	********************************************************************************
	<PRE>
	类名称 : VideoDetector
	功能 : 视频检测
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : VideoDetector iVD;
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class VideoDetector
	{
	public:
		explicit VideoDetector (
			const std::string& _fileName,
			const std::string& _modelPath = ".",
			const std::size_t _shotInterval = 100,
			const std::size_t _sceneInterval = 2000
		); //构造函数

		~VideoDetector(void); //析构函数

	public:

		const std::size_t getSceneNum (void); //返回场景数量

		const bool detectVideo (void); //检测视频

		const std::vector<double> getResultSet (void) const; //返回结果集合

		const std::vector<std::string> storeKeyFrames (
			const std::string& _folderPath, 
			const std::string& _imageName
		); //存储关键帧

	    static void help (void); //帮助信息

	private:

		std::string m_fileName; //视频文件名

		std::string m_modelPath; //模型路径

		std::size_t m_shotInterval; //镜头间隔

		std::size_t m_sceneInterval; //场景间隔

		std::vector<double> m_resultSet; //结果集合

	private:

		inline static std::ostream& _printLog (
			std::ostream& _os, const std::string& _msg); //打印信息
	};
}