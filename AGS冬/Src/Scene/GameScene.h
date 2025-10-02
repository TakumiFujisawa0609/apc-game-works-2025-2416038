#pragma once
#include "SceneBase.h"
class Camera;
class Grid;
class Camera;
class StageBase;
class Player;

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

private:

	Camera* camera_;
	Grid* grid_;
	StageBase* stage_;
	Player* player_;

};

