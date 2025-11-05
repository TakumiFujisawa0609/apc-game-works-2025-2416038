#pragma once
#pragma once
#include <vector>
#include <DxLib.h>
#include "../Gimmick/GimmickBase.h"

class GimmickQuiz : GimmickBase
{
	GimmickQuiz(void);
	~GimmickQuiz(void) override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	GimmickType GetType() const override;

private:

};

