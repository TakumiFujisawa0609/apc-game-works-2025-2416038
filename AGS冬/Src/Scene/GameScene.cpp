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
}

void GameScene::Release(void)
{
	player_->Release();
	delete player_;

	stageManager_->Release();
	delete stageManager_;

}


