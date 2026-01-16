#pragma once
#include "SceneBase.h"
class Camera;
class Player;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	Camera* camera_;
	Player* player_;

	void UpdateMenu(void);
	int cursorIndex_;   // ← メニューカーソル位置
	int moveDelay_;     // ← 押しっぱなし防止用タイマー

	// テレビ
	int tvModelId_ = 0;
	// 床
	int floorModelId_ = 0;

	// フォント
	int funwariFontHandle_;

	VECTOR pointLightPos_ = { 600.0f, 550.0f, 500.0f };

	bool isGameStart_ = false;
};
