/*! @file
********************************************************************************
<PRE>
ģ���� : �ؼ�֡
�ļ��� : KeyFrames.h 
����ļ� : KeyFrames.cpp
�ļ�ʵ�ֹ��� : �ؼ�֡������
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

//��Ƶ��������ռ�
namespace vd 
{
	/*! @class
	********************************************************************************
	<PRE>
	������ : KeyFrames
	���� : �ؼ�֡
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : KeyFrames iKF
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class KeyFrames 
	{
	public:

		KeyFrames (void); //���캯��

		explicit KeyFrames (
			const std::string& _videoName, 
			const std::size_t  _shotInterval = 200, 
			const std::size_t _beg = 0, 
			const std::size_t _end = UINT_MAX
		); //�������캯��

		virtual ~KeyFrames(); //��������

	public:

		void loadVideo (
			const std::string& _videoName, 
			const std::size_t  _shotInterval = 200, 
			const std::size_t _beg = 0, 
			const std::size_t _end = UINT_MAX
			); //������Ƶ

		virtual const bool extractKeyFrames (void) = 0; //��ȡ�ؼ�֡

		void storeKeyFrames (const std::string& _folderPath, const std::string& _imageName) const; //�洢�ؼ�֡

		const std::shared_ptr<std::vector<cv::Mat> > getKeyFrames (void) const; //���عؼ�֡

		const std::shared_ptr<std::vector<std::size_t> > getFramePositions (void) const; //����֡λ��

	protected:

		std::string m_videoName; //��Ƶ����

		std::size_t  m_shotInterval; //��ͷ���

		std::size_t m_beg; //��ʼ

		std::size_t m_end; //����

		bool m_isVideo; //�Ƿ�ɶ���Ƶ

		bool m_isKeyFrames; //�Ƿ���ȡ�ؼ�֡

		std::shared_ptr<std::vector<cv::Mat> > m_pKeyFrames; //�ؼ�֡����

		std::shared_ptr<std::vector<std::size_t> > m_pFramePositions; //֡λ��

	protected:

		inline void __resetData (void); //��������

		inline std::ostream& __printLog (
			std::ostream& os, const std::string& msg) const; //��ӡ��Ϣ

	};
}