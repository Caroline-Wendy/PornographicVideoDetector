/*! @file
********************************************************************************
<PRE>
ģ���� : ��������
�ļ��� : SceneIntegration.h 
����ļ� : SceneIntegration.cpp
�ļ�ʵ�ֹ��� : ��������������
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

//��Ƶ��������ռ�
namespace vd
{
	/*! @class
	********************************************************************************
	<PRE>
	������ : SceneIntegration
	���� : ��������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : SceneIntegration iSI(_fileName, _shotInterval, _sceneInterval);
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class SceneIntegration
	{
	public:

		explicit SceneIntegration (
			const std::string& _fileName, /*�ļ���*/
			const std::string& _modelPath, /*ģ��·��*/
			const std::size_t _shotInterval = 100, /*��ͷ���*/
			const std::size_t _sceneInterval = 2000 /*�������*/
		);

		~SceneIntegration (void);

		const double predictSceneProp (const std::size_t _beg = 0); //Ԥ�ⳡ��ֵ

		void getKeyInformation (
			std::vector<cv::Mat>& _keyFrames, 
			std::vector<std::size_t>& _framePositions
		); //��ȡ�ؼ�֡��Ϣ

	private:

		std::string m_fileName;	//�ļ���
		std::string m_modelPath; //ģ��·��
		std::size_t m_shotInterval; //��ͷ֡��
		std::size_t m_sceneInterval; //����֡��

		std::shared_ptr<std::vector<cv::Mat> > m_pKeyFrames; //�ؼ�֡��
		std::shared_ptr<std::vector<std::size_t> > m_framePositions; //�ؼ�֡λ��

	private:

		std::shared_ptr<std::vector<double> > 
			_detectImage (const cv::Mat& _keyFrame); //���ͼ��

		inline static std::ostream& _printLog (
			std::ostream& _os, const std::string& _msg); //��ӡ��Ϣ
	};
}