#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stagebase.h"
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

	//// ステージ
	//stage_ = new StageBase();
	//stage_->Init();

	// プレイヤー
	player_ = new Player();


	player_->Init();
	// カメラ
	camera_ = SceneManager::GetInstance()->GetCamera();
	camera_->SetFollow(player_);
	camera_->ChangeMode(Camera::MODE::FOLLOW);

	grid_ = new Grid();


	grid_->Init();
}

void GameScene::Update(void)
{
	player_->Update();
	//stage_->Update();
}

void GameScene::Draw(void)
{
	//stage_->Draw();
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

	/*stage_->Release();
	delete stage_;*/

}


