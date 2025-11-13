#pragma once
#include "SceneBase.h"
class Camera;
class Grid;
class Camera;
class HpManager;
class StageManager;
class GimmickManager;
class Player;
class ActorBase;
class Collision;
class Timer;

class GameScene : public SceneBase
{
public:
	// コンストラクタ
	GameScene(void);
	// デストラクタ
	~GameScene(void);
	// 初期化処理
	void Init(void) override;
	// 更新ステップ
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 解放処理
	void Release(void) override;

	Player* GetPlayer();
private:

	Camera* camera_;
	Grid* grid_;
	StageManager* stageManager_;
	Player* player_;
	HpManager* hpManager_;
	GimmickManager* gimmickManager_;
	Collision* collision_;
	Timer* timer_;


	void UpdatePauseMenu();  // ← ポーズ中メニュー処理
	void DrawPauseMenu();    // ← ポーズメニュー描画

	bool isPaused_ = false;
	int cursorIndex_ = 0;
	int moveDelay_ = 0;

	// ゲームオーバーフラグ
	bool isGameOver_;

	bool isClear_;
	float clearStartTime_;

};

