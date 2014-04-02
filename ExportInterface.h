/*! @file
********************************************************************************
<PRE>
模块名 : 导出接口
文件名 : ExportInterface.h
相关文件 : ExportInterface.cpp
文件实现功能 : 导出接口类声明
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
日 期              版本   修改人         修改内容 
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/

#ifndef PORNVIDEODETECTORDLL_H_
#define PORNVIDEODETECTORDLL_H_

#include <windows.h>

struct SceneInfo {
	char imagePath[1024]; //图片路径
	double prop; //置信度
	unsigned int bpos; //开始位置
	unsigned int epos; //结束位置
	double btime; //开始时间
	double etime; //结束时间
};

/*! @function
********************************************************************************
<PRE>
函数名 : PornVideoSceneNum
功能 : 色情视频场景数量
参数 : 
const char* const _fileName, 文件名;
const unsigned int _shotInterval, 镜头间隔;
const unsigned int _sceneShotNum, 场景数量;
返回值 : const unsigned int, 场景数量;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : num = PornVideoSceneNum (_fileName, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
作者 : C.L.Wang
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
函数名 : PornVideoDetector
功能 : 色情视频检测
参数 : 
double* _resultSet, 返回的结果集合;
const char* const _fileName, 文件名;
const char* const _modelPath, 模型路径;
const unsigned int _shotInterval, 镜头间隔;
const unsigned int _sceneShotNum, 场景数量;
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : 
isOK = PornVideoDetector (
_resultSet, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
作者 : C.L.Wang
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
函数名 : PornVideoDetector_Info
功能 : 色情视频检测-信息
参数 : 
SceneInfo* _infoSet, 场景信息集合;
const char* const _fileName, 文件名;
const char* const _folderPath, 文件夹路径;
const char* const _modelPath, 模型路径;
const unsigned int _shotInterval, 镜头间隔;
const unsigned int _sceneShotNum, 场景数量;
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : 
isOK = PornVideoDetector (
_infoSet, _folderPath, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
作者 : C.L.Wang
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
函数名 : PornVideoDetector_Help
功能 : 色情视频检测-帮助
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : PornVideoDetector_Help();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void PornVideoDetector_Help(void);

#endif /* PornVideoDetectorDll_H_ */