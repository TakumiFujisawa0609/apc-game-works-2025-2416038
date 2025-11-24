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

	int image_;
};
