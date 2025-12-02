#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Manager/SoundManager.h"
#include "Application.h"
#include "Common/FpsController.h"


Application* Application::instance_ = nullptr;

//const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
//const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
//const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";


Application::Application(void)
{
	isError_ = false;
	fpsController_ = nullptr;

}

Application::Application(const Application& ins)
{
}

Application::~Application()
{
}

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application* Application::GetInstance(void)
{
	return instance_;
}

void Application::Init(void)
{
	// アプリケーションの初期設定
	SetWindowText("Aso冬");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_BIT_DIPTH);
	//ChangeWindowMode(false);
	ChangeWindowMode(true);

	// FPS制御初期化
	fpsController_ = new FpsController(FRAME_RATE);

	// DxLib初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		isError_ = true;
		return;
	}

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();
	InputManager::GetInstance().Init();

	// シーン管理初期化
	SceneManager::CreateInstance();

	//// サウンド管理初期化
	SoundManager::CreateInstance();
	SoundManager::GetInstance();
	SoundManager::GetInstance()->Init();;


}

void Application::Run(void)
{
	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_0) == 0)
	{
		InputManager::GetInstance().Update();
		SceneManager::GetInstance()->Update();

		SceneManager::GetInstance()->Draw();

#ifdef _DEBUG

		// 平均FPS描画
		//fpsController_->Draw();

#endif // _DEBUG

		ScreenFlip();

		// 理想FPS経過待ち
		fpsController_->Wait();
	}

}

void Application::Destroy(void)
{
	// FPS制御メモリ解放
	delete fpsController_;

	// シーン管理破棄
	SceneManager::GetInstance()->Destroy();

	// サウンド管理破棄
	SoundManager::GetInstance()->DeleteInstance();


	if (DxLib_End() == -1)
	{
		isError_ = true;
		return;
	}

	delete instance_;

}

bool Application::IsError(void)
{
	return isError_;
}


