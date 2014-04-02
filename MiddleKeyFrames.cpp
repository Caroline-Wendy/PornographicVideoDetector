/*! @file
********************************************************************************
<PRE>
ģ���� : �ؼ�֡
�ļ��� : MiddleKeyFrames.cpp
����ļ� : MiddleKeyFrames.h
�ļ�ʵ�ֹ��� : �м�ؼ�֡��ʵ��
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
2014/03/27    1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#include "stdafx.h"

#include "MiddleKeyFrames.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : MiddleKeyFrames
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : MiddleKeyFrames iMKF;
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : MiddleKeyFrames
���� : �������캯��
���� : 
const string& _videoName, ��Ƶ��; 
const size_t  _shotInterval, ��ͷ���;
const size_t _beg, ��ʼλ��; 
const size_t _end, ��ֹλ��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : MiddleKeyFrames iMKF(videoName, shotInterval, beg, end);
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : ~MiddleKeyFrames
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iMKF.~MiddleKeyFrames();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
MiddleKeyFrames::~MiddleKeyFrames (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : extractKeyFrames
���� : ��ȡ�ؼ�֡
���� : void
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : isOK = extractKeyFrames();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool MiddleKeyFrames::extractKeyFrames (void)
{
	cv::VideoCapture iVC(m_videoName); //��Ƶ��

	const std::size_t framesNum(
		static_cast<std::size_t>(iVC.get(CV_CAP_PROP_FRAME_COUNT))); //֡��

	if (m_end > framesNum) { //Խ��������Ϊĩβ
		m_end = framesNum; 
	}

	const std::size_t sfNum = m_end - m_beg; //�������� 

	if (framesNum < (m_shotInterval/2+1)) { //��ͷ����
		__printLog(std::cerr, "The shot interval is too long, please reset.");
		return false;
	}

	/*����*/

	const double scale = iVC.get(CV_CAP_PROP_FRAME_HEIGHT)/FIXED_HIGHT;
	int srcRows = static_cast<int>(
		iVC.get(CV_CAP_PROP_FRAME_WIDTH)/scale);
	int srcCols = static_cast<int>(
		iVC.get(CV_CAP_PROP_FRAME_HEIGHT)/scale);
	cv::Size srcSize(srcRows, srcCols);

	/*��ȡ�ؼ�����*/

	for (std::size_t i=m_beg + (m_shotInterval/2); i<(m_beg+sfNum-1); i+=m_shotInterval)
	{
		cv::Mat frameTemp; //��Ҫÿ�ζ��½�frame

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