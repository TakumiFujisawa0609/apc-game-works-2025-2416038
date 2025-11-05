#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/StageManager.h"
#include "../Manager/Camera.h"
#include "../Manager/GimmickManager.h"
#include "../Object/Grid.h"
#include "../Object/Actor/Player.h"
#include "../Object/Actor/ActorBase.h"
#include "../UI/HpManager.h"
#include "../Common/Timer.h"
#include "../Common/Collision.h"
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
	gimmickManager_ = new GimmickManager();
	grid_ = new Grid();
	hpManager_ = new HpManager(player_);
	collision_ = new Collision();
	timer_ = new Timer();

	//// アクター配列に入れる
	//allActor_.push_back(player_);
	//// 全てのアクターを読み込み
	//for (auto actor : allActor_)
	//{
	//	// 読み込み
	//	actor->Load();
	//}

	stageManager_->Init();
	player_->Init();
	gimmickManager_->Init();

	// カメラ
	camera_ = SceneManager::GetInstance()->GetCamera();
	camera_->SetFollow(player_);
	camera_->ChangeMode(Camera::MODE::FOLLOW);

	grid_->Init();

	hpManager_->Init();
	collision_->Init(player_, stageManager_);

	// ゲームおーば判定
	isGameOver_ = false;

	isClear_ = false;
	clearStartTime_ = 0.0f;
}

void GameScene::Update(void)
{
	// ゲームオーバー判定
	if (player_->IsStateEnd()) {
		isGameOver_ = true;
	}

	auto& ins = InputManager::GetInstance();

	if (isGameOver_ || isClear_)
	{
		timer_->Pause();

		if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::B))
		{
			SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}

	if (isClear_) return; // クリアしてたら更新止める

	// --- ポーズ切り替え ---
	if (!isGameOver_ && !isClear_)
	{
		if (ins.IsTrgDown(KEY_INPUT_ESCAPE) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::START))
		{
			isPaused_ = !isPaused_;

			if (isPaused_)
				timer_->Pause();   // ★タイマー停止
			else
				timer_->Resume();  // ★タイマー再開
		}
	}


	// --- ポーズ中はゲーム停止してメニューのみ ---
	if (isPaused_)
	{
		UpdatePauseMenu();
		return;
	}

	stageManager_->Update();
	player_->Update();

	//// 全てのアクターを回す
	//for (auto actor : allActor_)
	//{
	//	// 更新処理
	//	actor->Update();

	//	// 当たり判定を取るか？
	//	if (actor)
	//	{
	//		// 当たり判定
	//		FieldCollision(actor);
	//	}
	//}

	gimmickManager_->Update();

	collision_->Update();

	// === 経過時間チェック ===
	if (timer_->IsOver(60.0f))  // ← 120秒経過でクリア
	{
		isClear_ = true;
		clearStartTime_ = timer_->GetElapsedSec();
		return;
	}
}

void GameScene::Draw(void)
{
	stageManager_->Draw();
	player_->Draw();

	//// 全てのアクターを回す
	//for (auto actor : allActor_)
	//{
	//	// 更新処理
	//	actor->Draw();
	//}

	gimmickManager_->Draw();
	collision_->Draw();

	hpManager_->Draw();

	//if (GetJoypadNum() == 0)
	//{
	//	DrawFormatString(300, 150, 0xffffff,"ESCでポーズ");
	//}
	//else
	//{
	//	DrawFormatString(280, 150, 0xffffff,"STARTボタンでポーズ");
	//}

	// グリッド線
	//grid_->Draw();

	float elapsed = timer_->GetElapsedSec();
	float remaining = 60.0f - elapsed;
	if (remaining < 0) remaining = 0;

	SetFontSize(50);
	DrawFormatString(0, 15, GetColor(255, 255, 255),
		"クリアまで : %.1f", remaining);
	SetFontSize(30);
	if (isPaused_)
	{
		DrawPauseMenu();
	}

	// --- クリア表示 ---
	if (isClear_)
	{
		// 画面中央に半透明黒＋文字
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetFontSize(39);
		DrawFormatString(430, 240, GetColor(255, 255, 0), "CLEAR");

		if (GetJoypadNum() == 0)
		{
			DrawFormatString(560, 480, GetColor(255, 255, 0), "Enterでタイトルへ→");
		}
		else
		{
			DrawFormatString(620, 480, GetColor(255, 255, 0), "Bでタイトルへ→");
		}
	}
	

	if (isGameOver_)
	{
		// 画面中央に半透明黒＋文字
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetFontSize(39);
		DrawFormatString(430, 240, GetColor(255, 255, 0), "GAME OVER");

		if (GetJoypadNum() == 0)
		{
			DrawFormatString(560, 480, GetColor(255, 255, 0), "Enterでタイトルへ→");
		}
		else
		{
			DrawFormatString(620, 480, GetColor(255, 255, 0), "Bでタイトルへ→");
		}
	}
}

void GameScene::Release(void)
{
	stageManager_->Release();
	delete stageManager_;

	player_->Release();
	delete player_;

	//// 全てのアクターを回す
	//for (auto actor : allActor_)
	//{
	//	// 更新処理
	//	actor->Release();
	//	delete actor;
	//}

	gimmickManager_->Release();
	delete gimmickManager_;

	collision_->Release();
	delete collision_;

	hpManager_->Release();
	delete hpManager_;

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
		ins.IsTrgDown(KEY_INPUT_RETURN) ||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::A);

	if (decide)
	{
		switch (cursorIndex_)
		{
		case 0: // GAME
			isPaused_ = false;
			timer_->Resume();  // ★タイマー再開
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
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < 3; i++)
	{
		int color = (i == cursorIndex_) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
		DrawFormatString(1000, 500 + i * 40, color, menu[i]);
	}

	DrawFormatString(950, 400, GetColor(255, 255, 255), "PAUSE MENU");
}
