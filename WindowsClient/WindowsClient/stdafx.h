// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// STL 헤더 파일
#include <vector>
#include <list>
#include <string>
#include <map>
#include <bitset>

#pragma comment(lib, "msimg32.lib")
// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.

#include "TimeManagent.h"
#include "KeyManagent.h"
#include "SceneManagent.h"

enum THREAD_ID
{
	THREAD_NULL = 0,
	THREAD_SCENE,
};

extern HWND _hWnd;
extern HINSTANCE _hInst;

typedef std::map<std::string, int> mapKeyList;

#define SAFE_DELETE(ptr)			do { if(ptr) delete (ptr);							(ptr) = nullptr; } while(false)
#define SAFE_DELETE_ARRAY(ptr)		do { if(ptr) delete[] (ptr);						(ptr) = nullptr; } while(false)
#define SAFE_RELEASE(ptr)			do { if(ptr) (ptr)->Release();										 } while(false)
#define SAFE_RELEASE_NULL(ptr)		do { if(ptr) (ptr)->Release();						(ptr) = nullptr; } while(false)
#define SAFE_RELEASE_DELETE(ptr)	do { if(ptr) { (ptr)->Release(); delete (ptr); }	(ptr) = nullptr; } while(false)
#define SAFE_UPDATE(ptr)			do { if(ptr) (ptr)->Update();										 } while(false)