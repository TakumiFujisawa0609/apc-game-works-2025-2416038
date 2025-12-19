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
#include "../Utility/AsoUtility.h"
#include "GameScene.h"


GameScene::GameScene(void)
{
}
GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stageManager_ = new StageManager();
	player_ = new Player();
	gimmickManager_ = new GimmickManager();

	grid_ = new Grid();
	hpManager_ = new HpManager(player_);
	collision_ = new Collision();

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
	collision_->Init(player_, stageManager_, gimmickManager_);

	// ゲームおーば判定
	isGameOver_ = false;

	isClear_ = false;
	clearStartTime_ = 0.0f;

	timer_ = new Timer();

	// 画像ロード
	timeBoxImg_ = LoadGraph("Data/Image/TimeBox.png"); // 時間の枠
	timeImg_ = LoadGraph("Data/Image/時計.png");	// 時計

	// フォントロード
	funwariFontHandle_ = CreateFontToHandle("Showcard Gothic", 80, 2, DX_FONTTYPE_ANTIALIASING);
	funwariFontHandle_2 = CreateFontToHandle("Showcard Gothic", 75, 2, DX_FONTTYPE_ANTIALIASING);
}
void GameScene::Update(void)
{
	if (player_->IsStateDead())
	{
		// カメラをズームさせる
		SceneManager::GetInstance()->GetCamera()->SetZoomTarget(0.5f); // プレイヤーに近づく
	}
	else if(isClear_)
	{
		// カメラをズームさせる
		SceneManager::GetInstance()->GetCamera()->SetZoomTarget(0.5f); // プレイヤーに近づく

		SceneManager::GetInstance()->GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
	}


	// 死んだかクリアかするとタイマーストップ
	if (player_->IsStateDead() || isClear_) timer_->Pause();
	// ゲームオーバー判定
	if (player_->IsStateEnd()) isGameOver_ = true;

	auto& ins = InputManager::GetInstance();
	if (isGameOver_ || isClear_)
	{
		if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::B))
		{
			SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}

	// === 経過時間チェック ===
	if (!isClear_ && timer_->IsOver(5.0f))  // ← 180秒経過でクリア
	{
		isClear_ = true;
		player_->ChangeState(Player::STATE::VICTORY);

		clearStartTime_ = timer_->GetElapsedSec();
	}

	// クリア後もプレイヤーだけ更新
	if (isClear_ || isGameOver_)
	{
		player_->Update();
		return; // その他の更新は止める
	}

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
	gimmickManager_->Update();

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


	collision_->Update();
}

void GameScene::Draw(void)
{
	
	stageManager_->Draw();
	player_->Draw();
	gimmickManager_->Draw();

	//// 全てのアクターを回す
	//for (auto actor : allActor_)
	//{
	//	// 更新処理
	//	actor->Draw();
	//}

	if (!isClear_ || !isGameOver_) collision_->Draw();

	// hp表示
	hpManager_->Draw();

	//SpotLight();

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

	// 制限時間描画
	DrawTime();

	SetFontSize(30);
	if (isPaused_)
	{
		DrawPauseMenu();
	}

	// --- クリア表示 ---
	if (isClear_)
	{
		/*SetFontSize(45);
		DrawFormatString(300, 240, GetColor(255, 255, 0), "CLEAR");

		if (GetJoypadNum() == 0)
		{
			DrawFormatString(310, 480, GetColor(255, 255, 0), "Enterでタイトルへ");
		}
		else
		{
			DrawFormatString(310, 480, GetColor(255, 255, 0), "Bでタイトルへ");
		}*/
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
	gimmickManager_->Release();
	delete gimmickManager_;

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

	collision_->Release();
	delete collision_;

	hpManager_->Release();
	delete hpManager_;

	// 画像の解放
	DeleteGraph(timeBoxImg_);
	DeleteGraph(timeImg_);

	// 作成したフォントデータを削除する
	DeleteFontToHandle(funwariFontHandle_);
}

Player* GameScene::GetPlayer()
{
	return player_;
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

void GameScene::SpotLight()
{
	// 下方向
	VECTOR forward = AsoUtility::DIR_D;
	// 位置
	VECTOR pos = { 300.0f, 1500.0f, 300.0f };

	// スポットライト
	ChangeLightTypeSpot(
		pos,
		forward,
		85.0f * DX_PI_F / 180.0f,  // 外側角度
		85.0f * DX_PI_F / 180.0f,  // 光の減衰が始まる位置
		3000.0f,					  // レンジ
		0.0f, 0.001f, 0.0f			  // 減衰係数
	);

	SetLightPosition(pos);
}

void GameScene::DrawTime()
{
	DrawGraph(0, 0, timeBoxImg_, true);

	float elapsed = timer_->GetElapsedSec();
	float remaining = 5.0f - elapsed;
	if (remaining < 0) remaining = 0;

	// 分と秒に変換
	int minutes = (int)remaining / 60;
	int seconds = (int)remaining % 60;

	SetFontSize(63);
	DrawFormatStringToHandle(
		175, 48,
		GetColor(255, 255, 255),
		funwariFontHandle_,
		"%d %02d",
		minutes,
		seconds
	);
	//DrawStringToHandle(207, 42, ":", GetColor(255, 255, 255), funwariFontHandle_2);
	DrawStringToHandle(217, 42, ":", GetColor(255, 255, 255), funwariFontHandle_2);
	// 時計画像
	DrawRotaGraph(120, 90, 0.06f, 0, timeImg_, true);
}
