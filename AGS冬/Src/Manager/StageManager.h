#pragma once
#include <vector>
#include <map>
#include <DxLib.h>
#include "../Object/Stage/StageBase.h"

class StageBase;

class StageManager
{
public:
	
	static constexpr int PANEL_NUM = 9;

	static constexpr float PANEL_SIZE = 800.0f;

	// コンストラクタ
	StageManager(void);
	// デストラクタ
	~StageManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// 連想配列
	std::map<StageBase::TYPE, std::vector<StageBase*>> stages_;

	// ステージモデルのハンドルID
	std::vector<int> stageModelIds_;

	VECTOR startPos_;   // 左上パネルの原点位置
};

