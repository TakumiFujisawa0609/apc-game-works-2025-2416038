#pragma once
#include <vector>
#include <DxLib.h>
#include "../Gimmick/GimmickBase.h"

class GimmickLaser : public GimmickBase
{
public:

	struct LaserInfo {
		float pos;      // 現在位置
		int direction;  // 0=左→右, 1=右→左, 2=奥→手前, 3=手前→奥
		int timer;      // 経過時間
		bool fired;     // 発射中か
		int modelHandle; // モデルハンドル
		VECTOR worldPos;
	};

	GimmickLaser(void);
	~GimmickLaser(void) override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	GimmickType GetType() const override;

	VECTOR GetPos() const;

private:

	std::vector<LaserInfo> lasers_;

	int modelId_;

	int spawnTimer_ = 0; // 次のレーザー生成用タイマー
	int activeCount_ = 0; // 今何本目か

	void InitLoad(void) override;
	void InitTransform(void) override;
	void InitPost(void) override;
};