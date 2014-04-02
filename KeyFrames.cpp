/*! @file
********************************************************************************
<PRE>
ģ���� : �ؼ�֡
�ļ��� : KeyFrames.cpp
����ļ� : KeyFrames.h
�ļ�ʵ�ֹ��� : �ؼ�֡��ʵ��
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
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/

#include "stdafx.h"

#include "KeyFrames.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : KeyFrames
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : KeyFrames iKF;
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
KeyFrames::KeyFrames (void) : 
	m_isVideo(false), /*δ����ͼ��*/
	m_isKeyFrames(false), /*δ��ȡ����*/
	m_pKeyFrames(new std::vector<cv::Mat>), /*�ؼ�ָ֡��*/
	m_pFramePositions(new std::vector<std::size_t>) /*֡λ��ָ��*/
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : KeyFrames
���� : �������캯��
���� : 
const std::string& _videoName, ��Ƶ��; 
const std::size_t  _shotInterval, ��ͷ���; 
const std::size_t _beg, ��ʼλ��; 
const std::size_t _end, ��ֹλ��;
����ֵ : ��
�׳��쳣 : exception, ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : KeyFrames iKF(_videoName);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
KeyFrames::KeyFrames (
	const std::string& _videoName, 
	const std::size_t  _shotInterval, 
	const std::size_t _beg, 
	const std::size_t _end
) :
	m_videoName(_videoName), /*��Ƶ*/
	m_shotInterval(_shotInterval), /*��ͷ���*/
	m_beg(_beg), //��ʼ
	m_end(_end), //����
	m_isKeyFrames(false), /*δ��ȡ����*/
	m_pKeyFrames(new std::vector<cv::Mat>), /*��������ָ��*/
	m_pFramePositions(new std::vector<std::size_t>) /*֡λ��ָ��*/
{
	cv::VideoCapture iVC(m_videoName);

	if (!iVC.isOpened()) { //������Ƶ
		__printLog(std::cerr, "Failed to open the video in <KeyFrames>!");
		throw std::exception("Video Error");
	}

	m_isVideo = true; //��Ƶ���Դ�

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~KeyFrames
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iKF.~KeyFrames();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
KeyFrames::~KeyFrames (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : loadVideo
���� : ������Ƶ
���� : 
const std::string& _videoName, ��Ƶ����;
const std::size_t  _shotInterval, ��ͷ���;
const std::size_t _beg, ��ʼλ��;
const std::size_t _end, ��ֹλ��;
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iKF.loadVideo(videoName);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void KeyFrames::loadVideo (
	const std::string& _videoName, 
	const std::size_t  _shotInterval, 
	const std::size_t _beg, 
	const std::size_t _end
)
{
	__resetData(); //��������

	m_videoName = _videoName;
	m_shotInterval = _shotInterval; /*��ͷ���*/
	m_beg = _beg; //��ʼ
	m_end = _end; //����

	cv::VideoCapture iVC(m_videoName);

	if (!iVC.isOpened()) { //������Ƶ
		__printLog(std::cerr, "Failed to open the video in <KeyFrames>!");
	}


	m_isVideo = true;

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : storeKeyFrames
���� : �洢�ؼ�֡
���� : 
const std::string& _folderPath, �ļ���·��; 
const std::string& _imageName, ͼ������;
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iKF.storeKeyFrames(folderPath, imageName);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void KeyFrames::storeKeyFrames (
	const std::string& _folderPath, 
	const std::string& _imageName) const
{
	if (_access(_folderPath.c_str(), 0)) { //�����ļ���
		__printLog(std::cerr, "Failed to open the folder! ");
		return;
	}

	if (!m_isKeyFrames) { //���Թؼ�֡
		__printLog(std::cerr, "Please extract key frames first! ");
		return;
	}

	std::size_t kfNum = m_pKeyFrames->size(); //�ؼ�֡��

	if (0 == kfNum) {
		__printLog(std::clog, "No key frames! ");
		return;
	}

	for (std::size_t i=0; i<kfNum; ++i) 
	{
		std::string name; //ͼ����

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
������ : getKeyFrames
���� : ���عؼ�֡
���� : void
����ֵ : const shared_ptr<vector<Mat> > �ؼ�֡
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : pKeyFrames = iKF.getKeyFrames();
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : getFramePositions
���� : ����֡λ��
���� : void
����ֵ : const shared_ptr<vector<size_t> > ֡λ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : pFramePositions = iKF.getFramePositions();
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : __resetData
���� : ��������
���� : void
����ֵ : void 
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __resetData();
--------------------------------------------------------------------------------
���� : C.L.Wang
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