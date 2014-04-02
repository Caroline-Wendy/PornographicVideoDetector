/*! @file
********************************************************************************
<PRE>
模块名 : 场景集成
文件名 : SceneIntegration.h 
相关文件 : SceneIntegration.cpp
文件实现功能 : 场景集成类声明
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

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <numeric>

#include <opencv.hpp>

#include "PornFeatures.h"
#include "MiddleKeyFrames.h"

//视频检测命名空间
namespace vd
{
	/*! @class
	********************************************************************************
	<PRE>
	类名称 : SceneIntegration
	功能 : 场景集成
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : SceneIntegration iSI(_fileName, _shotInterval, _sceneInterval);
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class SceneIntegration
	{
	public:

		explicit SceneIntegration (
			const std::string& _fileName, /*文件名*/
			const std::string& _modelPath, /*模型路径*/
			const std::size_t _shotInterval = 100, /*镜头间隔*/
			const std::size_t _sceneInterval = 2000 /*场景间隔*/
		);

		~SceneIntegration (void);

		const double predictSceneProp (const std::size_t _beg = 0); //预测场景值

		void getKeyInformation (
			std::vector<cv::Mat>& _keyFrames, 
			std::vector<std::size_t>& _framePositions
		); //提取关键帧信息

	private:

		std::string m_fileName;	//文件名
		std::string m_modelPath; //模型路径
		std::size_t m_shotInterval; //镜头帧数
		std::size_t m_sceneInterval; //场景帧数

		std::shared_ptr<std::vector<cv::Mat> > m_pKeyFrames; //关键帧集
		std::shared_ptr<std::vector<std::size_t> > m_framePositions; //关键帧位置

	private:

		std::shared_ptr<std::vector<double> > 
			_detectImage (const cv::Mat& _keyFrame); //检测图像

		inline static std::ostream& _printLog (
			std::ostream& _os, const std::string& _msg); //打印信息
	};
}