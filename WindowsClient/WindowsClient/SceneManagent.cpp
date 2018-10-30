#include "stdafx.h"

#include "Image.h"
#include "SceneManagent.h"

#include "TestScene.h"

namespace MHKLibrary
{
	GameNode* SceneManagent::_pReadyScene = nullptr;
	GameNode* SceneManagent::_pCurrentScene = nullptr;
	GameNode* SceneManagent::_pLoadingScene = nullptr;

	SceneManagent::SceneManagent(void)
		:	_dwLoadThreadID(THREAD_SCENE)
		,	_imgBackground(nullptr)
		,	_hdc(NULL)
	{
	}

	SceneManagent::~SceneManagent(void)
	{
	}

	DWORD CALLBACK LoadingThread(LPVOID prc)
	{
		if (FAILED(SceneManagent::_pLoadingScene->Init()))
		{
			SceneManagent::_pLoadingScene = nullptr;
		}
		SceneManagent::_pReadyScene = SceneManagent::_pLoadingScene;
		return NULL;
	}

	DWORD CALLBACK UnloadingThread(LPVOID prc)
	{
		SAFE_RELEASE(SceneManagent::_pLoadingScene);
		return NULL;
	}


	HRESULT SceneManagent::Init(void)
	{
		if (!_hdc)
		{
			_hdc = GetDC(_hWnd);
			if (!_imgBackground) return E_FAIL;
		}

		RECT rc;
		if (!_imgBackground)
		{
			_imgBackground = new Image;
			if (!_imgBackground) return E_FAIL;
			GetClientRect(_hWnd, &rc);
			if (FAILED(_imgBackground->Init(rc.right - rc.left, rc.bottom - rc.top))) return E_FAIL;
			SetBkMode(_imgBackground->GetMemDC(), TRANSPARENT);
		}

		RegistScene(SCENE_TEST, new TestScene);

		ChangeScene(SCENE_TEST);
		return S_OK;
	}

	void SceneManagent::Release(void)
	{
		_pReadyScene = nullptr;
		_pCurrentScene = nullptr;
		_pLoadingScene = nullptr;

		for (mapSceneIterator it = _mScenes.begin(); it != _mScenes.end(); it++)
		{
			SAFE_RELEASE_DELETE(it->second);
		}
		_mScenes.clear();

		SAFE_RELEASE_DELETE(_imgBackground);

		if (_hdc) ReleaseDC(_hWnd, _hdc);
		_hdc = NULL;
	}

	void SceneManagent::Update(void)
	{
		if (_pReadyScene)
		{
			_pLoadingScene = _pCurrentScene;
			_pCurrentScene = _pReadyScene;
			_pReadyScene = nullptr;
			CreateThread(nullptr, NULL, UnloadingThread, nullptr, NULL, &_dwLoadThreadID);
		}
		
		SAFE_UPDATE(_pCurrentScene);
	}

	void SceneManagent::Render(void)
	{
		if (!_imgBackground) return;

		_imgBackground->ColorNess(RGB(0, 0, 255));
		if (_pCurrentScene) _pCurrentScene->Render();
		_imgBackground->Render(_hdc);
	}


	GameNode* SceneManagent::RegistScene(SCENE_ID sceneID, GameNode * pScene)
	{
		if (!pScene) return nullptr;
		mapSceneIterator it = _mScenes.find(sceneID);
		if (_mScenes.end() != it) SAFE_RELEASE_DELETE(it->second);
		_mScenes[sceneID] = pScene;
		pScene->SetPHDC(_imgBackground->GetPHDC());
		return pScene;
	}

	HRESULT SceneManagent::ChangeScene(SCENE_ID sceneID)
	{
		if (nullptr != _pLoadingScene) return E_FAIL;
		mapSceneIterator it = _mScenes.find(sceneID);
		if (_mScenes.end() == it) return E_FAIL;

		_pLoadingScene = it->second;
		CreateThread(nullptr, NULL, LoadingThread, nullptr, NULL, &_dwLoadThreadID);
		return S_OK;
	}
}