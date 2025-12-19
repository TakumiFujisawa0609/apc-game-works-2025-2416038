#pragma once
#include "SceneBase.h"
class Camera;

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

	void UpdateMenu(void);
	int cursorIndex_;   // ← メニューカーソル位置
	int moveDelay_;     // ← 押しっぱなし防止用タイマー

	// テレビ
	int tvModelId_ = 0;
};
