/*! @file
********************************************************************************
<PRE>
ģ���� : �����Ƶ
�ļ��� : VideoDetector.cpp
����ļ� : VideoDetector.h
�ļ�ʵ�ֹ��� : ��Ƶ�����ʵ��
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
����                �汾   �޸���         �޸����� 
2014/03/26   1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#include "stdafx.h"

#include "VideoDetector.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : VideoDetector
���� : �������캯��
���� : 
const string& _fileName, �ļ���;
 const string& _modelPath, ģ��·��; 
const size_t _shotInterval, ��ͷ���;
const size_t _sceneInterval, �������;
����ֵ : ��
�׳��쳣 : exception, ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : VideoDetector(_fileName, _modelPath, _shotInterval, _sceneInterval);
--------------------------------------------------------------------------------
���� : C.L.Wang
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

	/*�ж��ļ��Ƿ����*/

	std::ifstream ifile;
	ifile.open(m_fileName, ios::in);
	if (ifile.fail()) {
		_printLog(std::cerr, "Failed to open the video!");
		throw std::exception("File Name Error");
	}
	ifile.close();

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

	/*�ж��ļ����Ƿ����*/

	if (_access(m_modelPath.c_str(), 0)) {
		_printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~VideoDetector
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iVD.~VideoDetector();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
VideoDetector::~VideoDetector(void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : detectVideo
���� : �����Ƶ
���� : void
����ֵ : const bool, �Ƿ�ɹ�
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : isOK = iVD.detectVideo();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool VideoDetector::detectVideo (void)
{
	/*�ж���Ƶ�Ƿ�ɶ�*/

	cv::VideoCapture iVC (m_fileName);

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //��֡��

	SceneIntegration iSI(m_fileName, m_modelPath, m_shotInterval, m_sceneInterval);

	double finalLabel (0.0); //�洢��Ƶ���
	std::size_t beg (0);
	std::size_t sceneNum(0);

	for (std::size_t j=0; j<framesNum; j+=m_sceneInterval)
	{
		/*��ȡ�ؼ�֡����*/

		finalLabel = iSI.predictSceneProp(beg);

		std::cout << " Scene Prop = " << finalLabel << std::endl;

		/*�洢���*/
		m_resultSet.push_back(finalLabel);

		beg += m_sceneInterval;

		++sceneNum;

	}

	return true;
}

/*! @function
********************************************************************************
<PRE>
������ : getResultSet
���� : ���ؽ������
���� : void
����ֵ : const vector<double>, �������
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : vd = iVD.getResultSet();
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : getSceneNum
���� : ���س�������
���� : void
����ֵ : const size_t, ��������
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : num = iVD.getSceneNum();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::size_t VideoDetector::getSceneNum (void)
{
	std::size_t sceneNum (0); //������

	cv::VideoCapture iVC (m_fileName);

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //�ؼ�֡��

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
������ : storeKeyFrames
���� : �洢�ؼ�֡
���� : 
const std::string& _folderPath, �ļ���·��;
const std::string& _imageName, ͼ������;
����ֵ : const vector<string>, ͼƬλ�ü���
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : vs = iVD.storeKeyFrames(_folderPath, _imageName);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::vector<std::string> VideoDetector::storeKeyFrames (
	const std::string& _folderPath, const std::string& _imageName)
{
	std::vector<std::string> names; //��������

	cv::VideoCapture cVC (m_fileName);

	const std::size_t framesNum(
		static_cast<std::size_t>(cVC.get(CV_CAP_PROP_FRAME_COUNT))); //��֡��

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
������ : help
���� : ������Ϣ
���� : void
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iVD.help();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void VideoDetector::help (void)
{
	const std::string version = "Version 1.0";
	std::cout << std::endl;
	std::cout << "ɫ����Ƶ��Ƶ�����(" << version << ")" << std::endl;
	std::cout << "����: C.L.Wang" << std::endl;
	std::cout << "Email: morndragon@126.com" << std::endl;
	std::cout << "��Ȩ�����ս���Ȩ��C.L.Wang����" << std::endl;
	std::cout << "�������ֹ�����޸ļ�ʹ��, �����޸Ļ�ʹ��, ���ʼ���ϵ." << std::endl;
	std::cout << "�ɰ�Ů��-ľ�����_��." << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "��������: ����ģ��->�����Ƶ." << std::endl;
	std::cout << "������ʹ��, ��Ҫ����ɫ����Ƶ���ģ��." << std::endl;

	std::cout << std::endl << std::endl;

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