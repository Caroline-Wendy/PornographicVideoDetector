/*! @file
********************************************************************************
<PRE>
ģ���� : ��Ƶ���
�ļ��� : VideoDetector.h 
����ļ� : VideoDetector.cpp
�ļ�ʵ�ֹ��� : ��Ƶ�����������
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
2014/03/25    1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>

#include "SceneIntegration.h"
#include "MiddleKeyFrames.h"

//��Ƶ��������ռ�
namespace vd 
{
		/*! @class
	********************************************************************************
	<PRE>
	������ : VideoDetector
	���� : ��Ƶ���
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : VideoDetector iVD;
	--------------------------------------------------------------------------------
	���� : C.L.Wang
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
		); //���캯��

		~VideoDetector(void); //��������

	public:

		const std::size_t getSceneNum (void); //���س�������

		const bool detectVideo (void); //�����Ƶ

		const std::vector<double> getResultSet (void) const; //���ؽ������

		const std::vector<std::string> storeKeyFrames (
			const std::string& _folderPath, 
			const std::string& _imageName
		); //�洢�ؼ�֡

	    static void help (void); //������Ϣ

	private:

		std::string m_fileName; //��Ƶ�ļ���

		std::string m_modelPath; //ģ��·��

		std::size_t m_shotInterval; //��ͷ���

		std::size_t m_sceneInterval; //�������

		std::vector<double> m_resultSet; //�������

	private:

		inline static std::ostream& _printLog (
			std::ostream& _os, const std::string& _msg); //��ӡ��Ϣ
	};
}