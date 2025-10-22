#pragma once
#include <vector>
#include <map>
#include <DxLib.h>
#include "../Object/Stage/StageBase.h"

class StageBase;

class StageManager
{
public:

	static constexpr int DIVISIONS = 3;

	static constexpr float STARTX = -300.0f;
	static constexpr float STARTZ = -300.0f;

	static constexpr float STEP = 600.0f;

	// コンストラクタ
	StageManager(void);
	// デストラクタ
	~StageManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);


	const std::map<StageBase::TYPE, std::vector<StageBase*>>& GetStages();

private:

	// 連想配列
	std::map<StageBase::TYPE, std::vector<StageBase*>> stages_;

	// ステージモデルのハンドルID
	std::vector<int> stageModelIds_;

	VECTOR startPos_;   // 左上パネルの原点位置
};

