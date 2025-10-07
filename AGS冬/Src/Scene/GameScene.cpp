#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/StageManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Actor/Player.h"
#include "GameScene.h"


GameScene::GameScene(void)
{
}
GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	// ステージ
	stageManager_ = new StageManager();
	player_ = new Player();
	grid_ = new Grid();

	stageManager_->Init();
	player_->Init();

	// カメラ
	camera_ = SceneManager::GetInstance()->GetCamera();
	camera_->SetFollow(player_);
	camera_->ChangeMode(Camera::MODE::FOLLOW);

	grid_->Init();
}

void GameScene::Update(void)
{
	auto& ins = InputManager::GetInstance();
	// --- ポーズ切り替え ---
	if (ins.IsTrgDown(KEY_INPUT_ESCAPE) ||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::START))
	{
		isPaused_ = !isPaused_;
	}

	// --- ポーズ中はゲーム停止してメニューのみ ---
	if (isPaused_)
	{
		UpdatePauseMenu();
		return;
	}

	player_->Update();
	stageManager_->Update();
}

void GameScene::Draw(void)
{
	stageManager_->Draw();
	player_->Draw();

	DrawFormatString(
		300, 200, 0xffffff,
		"Game Scene"
	);

	// グリッド線
	grid_->Draw();


	if (isPaused_)
	{
		DrawPauseMenu();
	}
}

void GameScene::Release(void)
{
	player_->Release();
	delete player_;

	stageManager_->Release();
	delete stageManager_;

}

void GameScene::UpdatePauseMenu()
{
	auto& ins = InputManager::GetInstance();
	VECTOR dir = ins.GetMenuInputDir();

	if (moveDelay_ > 0) moveDelay_--;

	if (moveDelay_ == 0)
	{
		if (dir.z < 0) { cursorIndex_--; moveDelay_ = 15; }
		if (dir.z > 0) { cursorIndex_++; moveDelay_ = 15; }

		if (cursorIndex_ < 0) cursorIndex_ = 2;
		if (cursorIndex_ > 2) cursorIndex_ = 0;
	}

	bool decide =
		ins.IsTrgDown(KEY_INPUT_SPACE) ||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);

	if (decide)
	{
		switch (cursorIndex_)
		{
		case 0: // GAME
			isPaused_ = false;
			break;

		case 1: // OPTION

			break;

		case 2: // TITLE
			SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::TITLE);
			break;
		}
	}
}

void GameScene::DrawPauseMenu()
{
	const char* menu[] = { "GAME", "OPTION", "TITLE" };

	// 半透明背景
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < 3; i++)
	{
		int color = (i == cursorIndex_) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
		DrawFormatString(600, 300 + i * 40, color, menu[i]);
	}

	DrawFormatString(580, 220, GetColor(255, 255, 255), "== PAUSE MENU ==");
}


