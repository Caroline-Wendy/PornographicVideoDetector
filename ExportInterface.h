/*! @file
********************************************************************************
<PRE>
ģ���� : �����ӿ�
�ļ��� : ExportInterface.h
����ļ� : ExportInterface.cpp
�ļ�ʵ�ֹ��� : �����ӿ�������
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

#ifndef PORNVIDEODETECTORDLL_H_
#define PORNVIDEODETECTORDLL_H_

#include <windows.h>

struct SceneInfo {
	char imagePath[1024]; //ͼƬ·��
	double prop; //���Ŷ�
	unsigned int bpos; //��ʼλ��
	unsigned int epos; //����λ��
	double btime; //��ʼʱ��
	double etime; //����ʱ��
};

/*! @function
********************************************************************************
<PRE>
������ : PornVideoSceneNum
���� : ɫ����Ƶ��������
���� : 
const char* const _fileName, �ļ���;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const unsigned int, ��������;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : num = PornVideoSceneNum (_fileName, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const unsigned int PornVideoSceneNum (
	const char* const _fileName,
	const unsigned int _shotInterval = 100, 
	const unsigned int _sceneShotNum = 20
);

/*! @function
********************************************************************************
<PRE>
������ : PornVideoDetector
���� : ɫ����Ƶ���
���� : 
double* _resultSet, ���صĽ������;
const char* const _fileName, �ļ���;
const char* const _modelPath, ģ��·��;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : 
isOK = PornVideoDetector (
_resultSet, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool PornVideoDetector (
	double* _resultSet,
	const char* const _fileName, 
	const char* const _modelPath,
	const unsigned int _shotInterval = 100, 
	const unsigned int _sceneShotNum = 20
);

/*! @function
********************************************************************************
<PRE>
������ : PornVideoDetector_Info
���� : ɫ����Ƶ���-��Ϣ
���� : 
SceneInfo* _infoSet, ������Ϣ����;
const char* const _fileName, �ļ���;
const char* const _folderPath, �ļ���·��;
const char* const _modelPath, ģ��·��;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : 
isOK = PornVideoDetector (
_infoSet, _folderPath, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool PornVideoDetector_Info (
	SceneInfo* _resultSet,
	const char* const _folderPath,
	const char* const _fileName, 
	const char* const _modelPath,
	const unsigned int _shotInterval = 100, 
	const unsigned int _sceneShotNum = 20
);

/*! @function
********************************************************************************
<PRE>
������ : PornVideoDetector_Help
���� : ɫ����Ƶ���-����
���� : void
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : PornVideoDetector_Help();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornVideoDetector_Help(void);

#endif /* PornVideoDetectorDll_H_ */