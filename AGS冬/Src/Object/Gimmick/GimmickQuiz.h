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

	TYPE GetType() const override;

private:

	void UpdateWave1(void) override;
	void UpdateWave2(void) override;
	void UpdateWave3(void) override;

};

