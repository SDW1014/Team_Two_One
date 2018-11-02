#pragma once
#include "Singleton.h"


namespace MHKLibrary
{
#define SCENE_MANAGENT		SceneManagent::GetInstance()

	enum SCENE_ID
	{
		SCENE_NULL = 0,
		SCENE_TEST,
	};

	class Scene;
	class Image;

	class SceneManagent : public Singleton<SceneManagent>
	{
	private:
		typedef std::map<SCENE_ID, Scene*> mapSceneList;
		typedef std::map<SCENE_ID, Scene*>::iterator mapSceneIterator;

		static Scene *_pReadyScene, *_pCurrentScene, *_pLoadingScene;

		mapSceneList	_mScenes;
		DWORD			_dwLoadThreadID;
		Image*			_imgBackground;
		HDC				_hdc;

	public:
		HRESULT Init(void);
		void Release(void);
		void Update(void);
		void Render(void);

		Scene* RegistScene(SCENE_ID sceneID, Scene* pScene);
		HRESULT ChangeScene(SCENE_ID sceneID);

		friend DWORD CALLBACK LoadingThread(LPVOID prc);
		friend DWORD CALLBACK UnloadingThread(LPVOID prc);

		SceneManagent(void);
		~SceneManagent(void);
	};
}

