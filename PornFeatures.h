/*! @file
********************************************************************************
<PRE>
模块名 : 色情特征
文件名 : PornFeatures.h 
相关文件 : PornFeatures.cpp
文件实现功能 : 色情特征类声明
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
2014/03/30   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include <io.h>

#include <opencv.hpp>

namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	类名称 : PornFeatures
	功能 : 色情特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : PornFeatures iPF(_image, _modelPath);
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class PornFeatures
	{
	public:

		explicit PornFeatures (const std::string& _modelPath);

		explicit PornFeatures (
			const cv::Mat& _image, //视频图像
			const std::string& _modelPath = "." //默认当前目录
		);

		~PornFeatures (void);

	public:

		void loadImage (const cv::Mat& image); //加载图像

		void showImage (void) const; //显示图像

	public:

		const bool loadModels (); //加载模型
		const bool extractFeatures (void); //提取特征
		const double predictPornImage (void); //预测图片

		static void help(void); //提供特征信息描述

	private:

		static const std::size_t FEATURE_NUM = 31;
		static const int FIXED_HIGHT = 400;

		static const std::string SKIN_MODEL; //皮肤模型
		static const std::string FACE_MODEL; //脸部模型
		static const std::string BREAST_MODEL; //胸部模型
		static const std::string CVT_MODEL; //决策树模型

	private:

		cv::Mat m_image; //图像
		std::string m_modelPath; //模型路径

		bool m_isImage; //是否包含图像
		bool m_isFeatures; //是否提取特征
		bool m_isModel; //是否加载模型

		cv::Mat m_pornFeatures; //色情特征

		cv::Mat m_skinHistModel; //肤色直方图
		cv::CascadeClassifier m_faceModel; //人脸检测模型
		cv::CascadeClassifier m_breastModel; //胸部检测模型
		CvRTrees m_rtrees; //决策树

	private:

		const double _skinColorProp (cv::Mat& _maskImg, double& _skinPoints); //肤色比例

		const double _imgEntropy (void); //图片熵

		const double _faceDetection (double& _faceSkinPoints, const cv::Mat& _maskImg); //人脸数量

		const double _breastDetection (const cv::Mat& _maskImg); //胸部数量

		void _imgMoments (std::vector<double>& _mv, const cv::Mat& _maskImg); //图像矩

		const double _skinRegion (
			std::vector<cv::Point>& _maxContour, 
			double& _skinRegionMaxArea, 
			const cv::Mat& _maskImg); //肤色区域数量

		void _maxRegion (cv::Point& _maxCenter, double& _maxRatio, 
			double& _maxAngle, std::vector<double>& _maxHumoments,
			double& _maxRoundness, double& _maxIrregularity,
			const std::vector<cv::Point>& _maxContour); //最大角点区域

		void _autoScale (void); //自动缩放
		void _resetImage (void); //重置数据

		inline static std::ostream& __printLog (
				std::ostream& _os, const std::string& _msg); //打印信息

	};

}