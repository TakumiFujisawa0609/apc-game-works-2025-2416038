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

	static constexpr VECTOR POS_MONITOR = { 300.0f, 220.0f, 2500.0f };
	static constexpr VECTOR SCL_MONITOR = { 9.0f, 9.0f, 9.0f };
	static constexpr VECTOR ROT_MONITOR = { -18 * (DX_PI_F / 180.0f), 0.0f, 0.0f };

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

