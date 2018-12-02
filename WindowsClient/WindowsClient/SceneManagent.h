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

		//¾À
		mapSceneList	_mScenes;
		DWORD			_dwLoadThreadID;
		//È­¸é
		Image*			_imgBackground;
		HDC				_hdc;

		//Ä¿¼­
		POINT _ptCursorPostion;
		bool _isCursorCenting;

	public:
		HRESULT Init(void);
		void Release(void);
		void Update(void);
		void Render(void);

		HRESULT ChangeScene(SCENE_ID sceneID);
		Scene* RegistScene(SCENE_ID sceneID, Scene* pScene);

		void SetCursorCenting(bool isCenting);

		friend DWORD CALLBACK LoadingThread(LPVOID prc);
		friend DWORD CALLBACK UnloadingThread(LPVOID prc);

		SceneManagent(void);
		~SceneManagent(void);
	};
}

