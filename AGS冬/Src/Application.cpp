#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
//#include "Manager/SoundManager.h"
#include "Application.h"
#//include "Common/FpsControl.h"


Application* Application::instance_ = nullptr;

//const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
//const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
//const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";


Application::Application(void)
{
	isError_ = false;
	fps_ = nullptr;

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
	SetWindowText("Aso");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_BIT_DIPTH);
	//ChangeWindowMode(false);
	ChangeWindowMode(true);


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
	//SoundManager::CreateInstance();
	//SoundManager::GetInstance();
	//SoundManager::GetInstance()->Init();

	//// FPS初期化
	//fps_ = new FpsControl;
	//fps_->Init();


}

void Application::Run(void)
{
	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_0) == 0)
	{
		//// フレームレート更新
		//// 1/60秒経過していないなら再ループさせる
		//if (!fps_->UpdateFrameRate()) continue;

		InputManager::GetInstance().Update();
		SceneManager::GetInstance()->Update();

		SceneManager::GetInstance()->Draw();

		//fps_->CalcFrameRate();	// フレームレート計算
		//fps_->DrawFrameRate();	// フレームレート描画


		ScreenFlip();
	}

}

void Application::Destroy(void)
{
	// シーン管理破棄
	SceneManager::GetInstance()->Destroy();

	//// サウンド管理破棄
	//SoundManager::GetInstance()->DeleteInstance();

	// フレームレート数
	delete fps_;

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


