/*! @file
********************************************************************************
<PRE>
模块名 : 色情特征
文件名 : PornFeatures.cpp
相关文件 : PornFeatures.h
文件实现功能 : 色情特征类实现
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

#include "stdafx.h"

#include "PornFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

const std::string PornFeatures::SKIN_MODEL = "/skin_hist_model.xml";
const std::string PornFeatures::FACE_MODEL = "/face_model.xml";
const std::string PornFeatures::BREAST_MODEL = "/breast_model.xml";
const std::string PornFeatures::CVT_MODEL = "/cvrtrees_model.xml";

/*! @function
********************************************************************************
<PRE>
函数名 : PornFeatures
功能 : 参数构造函数
参数 : 
const Mat& image, 图像;
const string& _modelPath, 模型路径;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : PornFeatures iPF(image, modelPath);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
PornFeatures::PornFeatures (const std::string& _modelPath) : 
	m_modelPath(_modelPath),
	m_isImage(false),
	m_isFeatures(false),
	m_isModel(false)
{
	if (_access(m_modelPath.c_str(), 0)) {
		__printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	//初始化色情特征向量, 必须为float
	m_pornFeatures = cv::Mat::zeros (1, FEATURE_NUM, CV_32FC1);
}

/*! @function
********************************************************************************
<PRE>
函数名 : PornFeatures
功能 : 参数构造函数
参数 : 
const Mat& image, 图像;
const string& _modelPath, 模型路径;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : PornFeatures iPF(image, modelPath);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
PornFeatures::PornFeatures (
	const cv::Mat& _image, 
	const std::string& _modelPath
) : 
	m_image(_image), 
	m_modelPath(_modelPath),
	m_isImage(true),
	m_isFeatures(false),
	m_isModel(false)
{
	if (m_image.empty()) {
		__printLog(std::cerr, "The image is empty. ");
		throw std::exception("Image Error");
	}

	if (m_image.channels() != 3) {
		__printLog(std::cerr, "The channels of image should be equal to 3. ");
		throw std::exception("Image Channel Error");
	}

	if (_access(m_modelPath.c_str(), 0)) {
		__printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	//初始化色情特征向量, 必须为float
	m_pornFeatures = cv::Mat::zeros (1, FEATURE_NUM, CV_32FC1);
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~PornFeatures
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iPF.~PornFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
PornFeatures::~PornFeatures() 
{
	m_rtrees.clear(); //决策树
}

/*! @function
********************************************************************************
<PRE>
函数名 : loadImage
功能 : 加载图像
参数 : const cv::Mat& _image, 图像;
返回值 : void
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iPF.loadImage(image);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::loadImage (const cv::Mat& _image) 
{
	m_image = _image;

	if (m_image.empty()) {
		__printLog(std::cerr, "The image is empty. ");
		throw std::exception("Image Error");
	}
	if (m_image.channels() != 3) {
		__printLog(std::cerr, "The channels of image should be equal to 3. ");
		throw std::exception("Image Channel Error");
	}

	/*初始化*/
	_resetImage();

	m_pornFeatures = cv::Mat::zeros (1, FEATURE_NUM, CV_32FC1);

	m_isImage = true;

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : showImage
功能 : 显示图像
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iPF.showImage();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::showImage (void) const
{
	if (m_image.empty()) { //测试图像
		__printLog(std::cerr, "Please load an image first!");
		return;
	}

	cv::imshow("Image", m_image); //显示图像
	cv::waitKey(0);

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : predictPornImage
功能 : 预测色情图像
参数 : void
返回值 : const double, 预测值;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : value = predictPornImage ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double PornFeatures::predictPornImage (void)
{
	if (!m_isFeatures || !m_isModel) {
		__printLog (std::cerr, 
			"The porn features are empty, so we cannot predict the result.");
		return -1.0;
	} 

	double result(0.0);
	result = m_rtrees.predict(m_pornFeatures);

	return result;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _loadModels
功能 : 加载模型
参数 : void
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : isOK = _loadModels ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool PornFeatures::loadModels ()
{
	std::string shmPath(m_modelPath);
	std::string fmPath(m_modelPath);
	std::string bmPath(m_modelPath);
	std::string ctmPath(m_modelPath);

	const std::string shmName(SKIN_MODEL);
	const std::string fmName(FACE_MODEL);
	const std::string bmName(BREAST_MODEL);
	const std::string ctmName(CVT_MODEL);

	shmPath.append(shmName);
	fmPath.append(fmName);
	bmPath.append(bmName);
	ctmPath.append(ctmName);

	//加载肤色正方图
	cv::FileStorage rfs(shmPath, cv::FileStorage::READ);
	rfs["skin_hist_model"] >> m_skinHistModel;
	rfs.release();
	if(m_skinHistModel.empty()) {
		__printLog(std::cerr, "Failed to load skin hist model.");
		return false;
	}

	//加载人脸模型
	if(!m_faceModel.load(fmPath)) {
		__printLog(std::cerr, "Failed to load face model.");
		return false;
	}

	//加载胸部模型
	if(!m_breastModel.load(bmPath)) {
		__printLog(std::cerr, "Failed to load breast model.");
		return false;
	}

	//加载决策树模型
	m_rtrees.load(ctmPath.c_str());

	m_isModel = true;

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : extractFeatures
功能 : 提取特征
参数 : void
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : isOK = iPF.extractFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool PornFeatures::extractFeatures (void) 
{
	if (m_image.empty() || m_isImage == false) {
		__printLog(std::cerr, "Please load image firstly.");
		return false;
	}

	_autoScale(); //自动设置图像比例

	cv::Mat maskImg; //肤色掩码图像, from _skinColorProp()
	double skinPoints(0.0); //肤色点数, from _skinColorProp()
	double faceSkinPoints(0.0); //脸部皮肤点数, from _faceDetection()
	std::vector<cv::Point> maxContour; //最大角点, from _skinRegion()
	double skinRegionMaxArea(0.0); //最大区域面积 from _skinRegion()

	/*特征0: 长宽比例*/
	double cr_ratio = static_cast<double>(m_image.cols) / 
		static_cast<double>(m_image.rows);
	m_pornFeatures.at<float>(0, 0) = static_cast<float>(cr_ratio); //长宽比

	/*特征1: 肤色比例*/
	m_pornFeatures.at<float>(0, 1) = 
		static_cast<float>(_skinColorProp(maskImg, skinPoints));

	/*特征2: 图片熵*/
	m_pornFeatures.at<float>(0, 2) = static_cast<float>(_imgEntropy());

	/*特征3: 人脸数量*/
	m_pornFeatures.at<float>(0, 3) = 
		static_cast<float>(_faceDetection(faceSkinPoints, maskImg));

	/*特征4: 脸部肤色比例*/
	double faceSkinProp (0.0);
	if (skinPoints > 0)
		faceSkinProp = faceSkinPoints / skinPoints;
	m_pornFeatures.at<float>(0, 4) = static_cast<float>(faceSkinProp);

	/*特征5: 胸部数量*/
	m_pornFeatures.at<float>(0, 5) = static_cast<float>(_breastDetection(maskImg));

	/*特征6: 私部数量*/
	m_pornFeatures.at<float>(0, 6) = 0.0; //未完成

	/*特征7-16: 图像矩*/
	std::vector<double> mv;
	_imgMoments(mv, maskImg); //计算图像的矩
	for (int i=0; i<10; ++i)
		m_pornFeatures.at<float>(0, 7+i) = static_cast<float>(mv[i]);

	/*特征17: 皮肤区域数量*/
	m_pornFeatures.at<float>(0, 17) = 
		static_cast<float>(_skinRegion(maxContour, skinRegionMaxArea, maskImg));

	/*特征18: 最大轮廓面积比*/
	double contourAreaProp(0.0);
	if (skinPoints > 0)
		contourAreaProp = skinRegionMaxArea / skinPoints;
	m_pornFeatures.at<float>(0, 18) = static_cast<float>(contourAreaProp);

	cv::Point maxCenter;
	double centerDistance(0.0), maxRatio(0.0), maxAngle(0.0);
	double maxRoundness(0.0), maxIrregularity(0.0);
	std::vector<double> maxHumoments;
	_maxRegion (maxCenter, maxRatio, maxAngle, 
		maxHumoments, maxRoundness, maxIrregularity, maxContour);

	/*特征19-21: 最大角点区域*/
	centerDistance = std::sqrt(std::pow((m_image.rows*0.5-maxCenter.x), 2.0)
		+std::pow((m_image.cols*0.5-maxCenter.y), 2.0));
	m_pornFeatures.at<float>(0, 19) = static_cast<float>(centerDistance); //中心距离
	m_pornFeatures.at<float>(0, 20) = static_cast<float>(maxRatio); //长短轴比
	m_pornFeatures.at<float>(0, 21) = static_cast<float>(maxAngle); //旋转角度

	/*特征22-28: 最大角点区域的Hu矩*/
	for (int i=0; i<7; ++i)
		m_pornFeatures.at<float>(0, 22+i) = static_cast<float>(maxHumoments[i]);

	/*特征29: 最大角点区域的圆度*/
	m_pornFeatures.at<float>(0, 29) = static_cast<float>(maxRoundness);

	/*特征30: 最大角点区域的不规则度*/
	m_pornFeatures.at<float>(0, 30) = static_cast<float>(maxIrregularity);

	m_isFeatures = true;

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _skinColorProp
功能 : 肤色比例
参数 : 
Mat& _maskImg, 肤色掩码图像, 返回;
double& _skinPoints, 肤色点数, 返回;
返回值 : const double, 肤色比例;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : prop = _skinColorProp (maskImg, skinPoints);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double PornFeatures::_skinColorProp (cv::Mat& _maskImg, double& _skinPoints)
{
	cv::Mat backProject; //后射输出矩阵
	cv::Mat hsvImage;
	double skinProp;

	cv::cvtColor(m_image, hsvImage, CV_BGR2HSV);

	int channels[] = {0, 1}; //使用HS通道
	float hranges[] = { 0, 180 };
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges }; //HS的范围

	cv::calcBackProject(&hsvImage, 1, channels, m_skinHistModel, backProject, ranges);

	/*全局肤色掩码mask*/
	cv::threshold(backProject, _maskImg, 4, 1, CV_THRESH_BINARY);/*把大于4的都置为1*/

	cv::blur(_maskImg, _maskImg, cv::Size(3,3));
	cv::morphologyEx(_maskImg, _maskImg, MORPH_OPEN, 0);
	cv::morphologyEx(_maskImg, _maskImg, MORPH_CLOSE, 0);

	/*计算全局肤色点数*/
	_skinPoints = static_cast<double>(cv::countNonZero(_maskImg));
	skinProp =  _skinPoints / static_cast<double>(hsvImage.rows*hsvImage.cols);

	return skinProp;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _imgEntropy
功能 : 图片熵
参数 : void
返回值 : const double, 图片熵;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : entropy = _imgEntropy ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double PornFeatures::_imgEntropy (void)
{
	
	cv::Mat gryImg; //灰度图像
	cv::Mat hist;
	double entropy (0.0);

	cv::cvtColor(m_image, gryImg, CV_BGR2GRAY);

	int bins = 256;
	int histSize[] = {bins};
	float granges[] = {0, 255};
	const float* ranges[] = { granges };
	int channels[] = {0};

	cv::calcHist(&gryImg, 1, channels, Mat(), hist, 1, histSize, ranges);

	/*整型转换为double型*/
	cv::Mat normHist = cv::Mat (hist.size(), CV_64FC1);
	for (int i=0; i<normHist.rows; ++i) {
		normHist.at<double>(i,0) = static_cast<double>(hist.at<int>(i,0));
	}

	cv::normalize(normHist, normHist, 1.0, 0.0, NORM_L1); //归一化

	/*计算熵*/
	for(int i=0; i<bins; ++i){
		double temp = normHist.at<double>(i, 0);
		if(temp!=0) entropy -= temp*std::log(temp)/std::log(2.0);
	}

	return entropy;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _faceDetection
功能 : 人脸数量
参数 : 
double& _faceSkinPoints, 脸部肤色点数, 返回;
const Mat& _maskImg, 肤色掩码图像;
返回值 : const double, 人脸数量;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : num = _faceDetection (faceSkinPoints, maskImg);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double PornFeatures::_faceDetection (
	double& _faceSkinPoints, const cv::Mat& _maskImg)
{
	double faceNum(0.0); //人脸数量
	double skinThreshold(0.16); //脸部肤色阈值
	bool tryFlip(true); //翻转检测

	int faceTemp(0); //人脸参数
	std::vector<cv::Rect> faces, faces2;
	cv::Mat gryImg; //灰度图像

	cv::cvtColor(m_image, gryImg, CV_BGR2GRAY);

	m_faceModel.detectMultiScale(gryImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		|CV_HAAR_DO_ROUGH_SEARCH //效果最好
		//|CV_HAAR_SCALE_IMAGE
		//|CV_HAAR_DO_CANNY_PRUNING
		,
		cv::Size(30, 30) );
	if( tryFlip ) 
	{
		cv::flip(gryImg, gryImg, 1); //翻转
		m_faceModel.detectMultiScale( gryImg, faces2,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_SCALE_IMAGE
			//|CV_HAAR_DO_CANNY_PRUNING
			,
			cv::Size(30, 30) );
		for(std::vector<cv::Rect>::const_iterator r = faces2.begin();
			r != faces2.end(); r++ )
		{
			cv::Rect rec = cv::Rect(gryImg.cols-(r->x)-(r->width), 
				(r->y), (r->width), (r->height));
			faces.push_back(rec);
		}
	}

	for(std::vector<cv::Rect>::const_iterator r = faces.begin();
		r != faces.end(); ++r)
	{
		cv::Mat mask_ROI = _maskImg(*r);
		cv::Scalar sc_face_skin = cv::sum(mask_ROI);
		double face_skin = sc_face_skin[0];

		double skinRatio = face_skin/(r->width*r->height); //脸部肤色比例
		if(skinRatio > skinThreshold )
		{
			_faceSkinPoints += face_skin; //计算全局脸部肤色点数
			++faceTemp;
		}
	}

	faceNum = static_cast<double>(faceTemp)/2.0;

	return faceNum;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _breastDetection
功能 : 胸部数量
参数 : const Mat& _maskImg, 肤色掩码图像
返回值 : const double, 胸部数量;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : num = _breastDetection (maskImg);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double PornFeatures::_breastDetection (const Mat& _maskImg)
{
	double breastNum(0.0); //人脸数量
	double skinThreshold(0.3); //脸部肤色阈值
	bool tryFlip(true); //翻转检测

	int breast_temp(0); //人脸参数
	std::vector<cv::Rect> breasts, breasts2;

	cv::Mat gryImg; //灰度图像

	cv::cvtColor(m_image, gryImg, CV_BGR2GRAY);

	m_breastModel.detectMultiScale(gryImg, breasts,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		|CV_HAAR_DO_ROUGH_SEARCH //效果最好
		//|CV_HAAR_SCALE_IMAGE
		//|CV_HAAR_DO_CANNY_PRUNING
		,
		cv::Size(10, 10) ); //胸部最小值
	if( tryFlip ) 
	{
		cv::flip(gryImg, gryImg, 1); //翻转
		m_breastModel.detectMultiScale( gryImg, breasts2,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_SCALE_IMAGE
			//|CV_HAAR_DO_CANNY_PRUNING
			,
			cv::Size(10, 10) );
		for( std::vector<cv::Rect>::const_iterator r = breasts2.begin();
			r != breasts2.end(); r++ )
		{
			cv::Rect rec = cv::Rect(gryImg.cols - (r->x) - (r->width),
				r->y, r->width, r->height);
			breasts.push_back(rec);
		}
	}

	for( std::vector<cv::Rect>::const_iterator r = breasts.begin();
		r != breasts.end(); ++r)
	{
		cv::Mat mask_ROI = _maskImg(*r);
		cv::Scalar sc_breast_skin = cv::sum(mask_ROI);
		double breast_skin = sc_breast_skin[0];

		double skinRatio = breast_skin/(r->width*r->height); //胸部肤色比例
		if(skinRatio > skinThreshold )
		{
			++breast_temp;
		}
	}

	breastNum = static_cast<double>(breast_temp)/2.0;

	return breastNum;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _imgMoments
功能 : 图像的矩
参数 : 
vector<double>& _mv, 图像的矩, 10维, 返回;
const cv::Mat& _maskImg, 肤色掩码图像;
返回值 : void;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : _imgMoments (mv, maskImg);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::_imgMoments (std::vector<double>& _mv, const cv::Mat& _maskImg)
{
	_mv.clear();

	cv::Moments moments;
	moments = cv::moments(_maskImg, true);

	_mv.push_back(moments.m00);
	_mv.push_back(moments.m10);
	_mv.push_back(moments.m01);
	_mv.push_back(moments.m11);
	_mv.push_back(moments.m20);
	_mv.push_back(moments.m02);
	_mv.push_back(moments.m21);
	_mv.push_back(moments.m12);
	_mv.push_back(moments.m30);
	_mv.push_back(moments.m03);

	if (_mv.size() != 10)
		__printLog(cerr, "The size of moment vector should be equal to 10.");

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _skinRegion
功能 : 皮肤区域
参数 : 
vector<Point>& _maxContour, 最大角点, 返回;
double& _skinRegionMaxArea, 最大区域面积, 返回;
const Mat& _maskImg, 肤色掩码图像;
返回值 : const double, 肤色面积值;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : region = _skinRegion (maxContour, skinRegionMaxArea, maskImg);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double PornFeatures::_skinRegion (
	std::vector<cv::Point>& _maxContour, 
	double& _skinRegionMaxArea, 
	const cv::Mat& _maskImg)
{
	double regionNum(0.0); //肤色区域数目
	unsigned int contourNum(0); //角点数量
	unsigned int smallContour(0); //较小角点区域

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	/*检查肤色轮廓*/
	cv::findContours(_maskImg, contours, 
		CV_RETR_CCOMP/*建立两个等级的轮廓*/ ,
		CV_CHAIN_APPROX_SIMPLE /*矩形轮廓只需4个点来保存轮廓信息*/
		);

	contourNum = contours.size();
	if (!contours.empty()) {
		_maxContour = contours[0];
	}

	for(std::vector<std::vector<cv::Point> >::const_iterator c=contours.begin();
		c!=contours.end(); ++c)
	{
		double areaTmp = cv::contourArea(*c); //获取轮廓面积
		int cnt = (*c).size(); //角点的点数
		if(areaTmp > _skinRegionMaxArea) {
			_skinRegionMaxArea = areaTmp; //最大轮廓面积
			_maxContour = *c; //最大轮廓
		}
		if(cnt<60 || areaTmp<100) {
			++smallContour;
		}
	}

	if(_maxContour.size() < 5) { //最大角点不存在, 设为空(5)
		std::vector<cv::Point> temp(5);
		_maxContour = temp;
	}

	regionNum = static_cast<double>(contourNum - smallContour);

	return regionNum;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _maxRegion
功能 : 最大区域
参数 : 
Point& _maxCenter, 最大中心, 返回;
double& _maxRatio, 最大比例, 返回;
double& _maxAngle, 最大角度, 返回;
vector<double>& _maxHumoments, 最大Hu矩, 返回;
double& _maxRoundness, 最大圆度, 返回;
double& _maxIrregularity, 最大不规则度, 返回;
const vector<Point>& _maxContour, 最大角点, 传入参数;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : _maxRegion (_maxCenter, _maxRatio, _maxAngle, 
_maxHumoments, _maxRoundness, _maxIrregularity, _maxContour);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::_maxRegion (cv::Point& _maxCenter, double& _maxRatio, 
	double& _maxAngle, std::vector<double>& _maxHumoments,
	double& _maxRoundness, double& _maxIrregularity,
	const std::vector<cv::Point>& _maxContour
)
{
	_maxHumoments.clear();

	_maxRatio = 0.0; //长宽比
	_maxAngle = 0.0; //角度
	_maxRoundness = 0.0; //圆度
	_maxIrregularity = 0.0; //不规则度

	cv::RotatedRect contourRect; //椭圆拟合型状
	cv::Size maxSize; //拟合图像的长宽

	if(_maxContour.size() < 5) {
		__printLog(std::clog, "The size of max contour is small!");
	} else {
		contourRect = cv::fitEllipse(_maxContour);
	}

	maxSize = contourRect.size;

	_maxRatio = static_cast<double>(maxSize.width)/static_cast<double>(maxSize.height);
	_maxAngle = contourRect.angle;
	_maxCenter = contourRect.center;

	cv::Moments moments;
	moments = cv::moments(_maxContour);
	cv::HuMoments(moments, _maxHumoments);

	/*Fourier*/
	double q(0.0), r(0.0), s(0.0), t(0.0);
	int xt(0), yt(0), xt2(0), yt2(0);
	const double MYPI = 3.1415926;
	int cnt = _maxContour.size();
	for(int i=0; i<cnt; ++i){
		double temp = 2*MYPI*i/static_cast<double>(cnt);
		q += _maxContour[i].x * cos(temp); 
		r += _maxContour[i].y*sin(temp); 
		s += _maxContour[i].y*cos(temp); 
		t += _maxContour[i].x*sin(temp); 

		xt += _maxContour[i].x;
		yt += _maxContour[i].y;
		xt2 += static_cast<int>(std::pow(_maxContour[i].x, 2.0));
		yt2 += static_cast<int>(std::pow(_maxContour[i].y, 2.0));
	}

	double sigma = (xt2+yt2)/static_cast<double>(cnt)
		- std::pow(xt/static_cast<double>(cnt), 2.0)
		- std::pow(yt/static_cast<double>(cnt), 2.0);

	double mrTemp(0.0), mrTemp2(0.0);
	mrTemp = std::pow((q+r), 2.0)+pow((s-t), 2.0);
	mrTemp2 = std::pow((q-r), 2.0)+pow((s+t), 2.0);
	if(mrTemp2>0)
		_maxRoundness = mrTemp/mrTemp2;

	double miTemp(0.0);
	miTemp = (q*q+r*r+s*s+t*t);
	if(sigma!=0)
		_maxIrregularity = 1.0 - 2.0*miTemp/sigma;

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _autoScale
功能 : 缩放图像
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : _autoScale ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::_autoScale (void)
{
	const double	m_scale = static_cast<double>(m_image.rows)/FIXED_HIGHT;

	int srcRows = static_cast<int>(static_cast<double>(m_image.rows)/m_scale);
	int srcCols = static_cast<int>(static_cast<double>(m_image.cols)/m_scale);
	cv::Size srcSize(srcRows, srcCols);
	cv::resize(m_image, m_image, srcSize);

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _resetImage
功能 : 重置图像
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : _resetImage ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::_resetImage (void)
{	
	m_isFeatures = false;
	m_pornFeatures.release();

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __printLog
功能 : 输出信息
参数 : 
ostream& _os, 输出格式; 
const std::string& _msg, 输出内容;
返回值 : std::ostream&;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
std::ostream& PornFeatures::__printLog (
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

/*! @function
********************************************************************************
<PRE>
函数名 : __printLog
功能 : 输出信息
参数 : 
ostream& _os, 输出格式; 
const std::string& _msg, 输出内容;
返回值 : std::ostream&;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornFeatures::help(void)
{
	const std::string version = "Version 1.0";

	std::cout << std::endl;
	std::cout << "色情图像检测器(" << version << ")" << std::endl;
	std::cout << "Author: C.L.Wang" << std::endl;
	std::cout << "Email: morndragon@126.com" << std::endl;
	std::cout << "版权及解释权归C.L.Wang所有" << std::endl;
	std::cout << "本程序禁止擅自使用及修改, 如需要使用, 请邮件联系." << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "方法描述:";
	std::cout << "加载模型->提取图像特征->检测图像是否为色情" <<std::endl;
	std::cout << "特征描述: " << std::endl;
	std::cout << "feature[0]:图像长宽比" << std::endl;
	std::cout << "feature[1]:皮肤像素占整体像素的比例" << std::endl;
	std::cout << "feature[2]:整幅图像的熵(判断是否是动画图片)" << std::endl;
	std::cout << "feature[3]:检测到的人脸数量" << std::endl;
	std::cout << "feature[4]:人脸肤色所占肤色像素的比例" << std::endl;
	std::cout << "feature[5]:检测到的胸部数量" << std::endl;
	std::cout << "feature[6]:检测到的私部数量(未完成)" << std::endl;
	std::cout << "feature[7]~feature[16]: 皮肤区域的矩" << std::endl;
	std::cout << "feature[17]:皮肤区域的数量" << std::endl;
	std::cout << "feature[18]:最大轮廓面积占总肤色面积的比例" << std::endl;
	std::cout << "feature[19]:最大轮廓中心到图像中心的距离" << std::endl;
	std::cout << "feature[20]:最大轮廓椭圆拟合长短轴之比" << std::endl;
	std::cout << "feature[21]:最大轮廓椭圆拟合主轴角度" << std::endl;
	std::cout << "feature[22]~feature[28]: 最大角点的Hu矩" << std::endl;
	std::cout << "feature[29]:最大轮廓圆度" << std::endl;
	std::cout << "feature[30]:最大轮廓不规则度" << std::endl;

	std::cout << std::endl << std::endl;

	return;
}
