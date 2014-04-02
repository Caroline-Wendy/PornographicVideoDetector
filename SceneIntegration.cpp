/*! @file
********************************************************************************
<PRE>
ģ���� : ��������
�ļ��� : SceneIntegration.cpp
����ļ� : SceneIntegration.h
�ļ�ʵ�ֹ��� : ����������ʵ��
���� : C.L.Wang
Email: morndragon@126.com
�汾 : 1.0
--------------------------------------------------------------------------------
���̰߳�ȫ�� : ��
�쳣ʱ��ȫ�� : ��
--------------------------------------------------------------------------------
��ע : ��
--------------------------------------------------------------------------------
�޸ļ�¼ :  
�� ��              �汾   �޸���         �޸����� 
2014/03/28    1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#include "stdafx.h"

#include "SceneIntegration.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : SceneIntegration
���� : �������캯��
���� : 
const string& _fileName, ��Ƶ��;
const size_t  _shotInterval, ��ͷ���;
const size_t _sceneInterval, �������;
����ֵ : ��
�׳��쳣 : exception, ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : SceneIntegration iSI(fileName, shotInterval, sceneInterval);
--------------------------------------------------------------------------------
���� : C.L.Wang
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

	/*�ж��ļ��Ƿ����*/

	std::ifstream ifile;
	ifile.open(m_fileName, ios::in);
	if (ifile.fail()) {
		_printLog(std::cerr, "Failed to open the video!");
		throw std::exception("File Name Error");
	}
	ifile.close();

	/*�ж��ļ��Ƿ��ܴ�*/

	if (_access(m_modelPath.c_str(), 0)) {
		_printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	/*��ͷ����ж�*/

	if (m_shotInterval < 1 || m_shotInterval > maxShotInterval) {
		_printLog(std::cerr, "The value of the shot interval is invalid.");
		throw std::exception("Shot Interval Error");
	}

	/*��������ж�*/

	if (m_sceneInterval < 1 || m_sceneInterval > maxSceneInterval) {
		_printLog(std::cerr, "The value of the scene interval is invalid.");
		throw std::exception("Scene Interval Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~SceneIntegration
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iSI.~SceneIntegration ();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
SceneIntegration::~SceneIntegration (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : predictSceneProp
���� : Ԥ�ⳡ������
���� : const size_t _beg, ������ʼλ��
����ֵ : const double, ��������
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : value = iSI.predictSceneProp (_beg);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const double SceneIntegration::predictSceneProp (const std::size_t _beg) //Ԥ�ⳡ��ֵ
{
	double finalResult(0.0);
	std::vector<double> resultSet;

	cv::VideoCapture cVC (m_fileName); //��Ƶ��ȡ

	if (!cVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video!");
		return -1.0;
	}

	std::size_t framesNum = 
		static_cast<std::size_t>(cVC.get(CV_CAP_PROP_FRAME_COUNT)); //��֡��

	MiddleKeyFrames iMKF(m_fileName, m_shotInterval, _beg, _beg+m_sceneInterval);

	if(!iMKF.extractKeyFrames() ) {
		_printLog(std::cerr, "Failed to extract key frames.");
		return 1.0;
	}

	m_pKeyFrames = iMKF.getKeyFrames();
	m_framePositions = iMKF.getFramePositions();

	double value(0.0); //��ʱֵ
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
������ : getKeyInformation
���� : ��ȡ�ؼ�֡��Ϣ
���� : void
����ֵ : 
shared_ptr<vector<Mat> >& _keyFrames, �ؼ�֡����;
vector<size_t>& _framePositions, �ؼ�֡λ�ü���;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : getKeyFrames();
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : __printLog
���� : ��ӡ��Ϣ
���� : 
_os, ��ӡ��ʽ; 
_msg, ��ӡ����;
����ֵ : std::ostream& ��ӡ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
���� : C.L.Wang
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