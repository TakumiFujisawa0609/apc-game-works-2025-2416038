#pragma once
#include <DxLib.h>
#include "SceneBase.h"

class Tutorial : public SceneBase
{

public:

	// コンストラクタ
	Tutorial(void);

	// デストラクタ
	~Tutorial(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	int tutorialImg_[3];
	int pageIndex_;   // ← 今何ページ目か
};
