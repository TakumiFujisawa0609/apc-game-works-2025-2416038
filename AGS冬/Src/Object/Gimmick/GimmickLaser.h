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
		float height;   // 高さ
	};

	GimmickLaser(void);
	~GimmickLaser(void) override;

	//void Update() override;
	void Draw() override;
	void Release() override;

	TYPE GetType() const override;

	VECTOR GetPos() const override;

	std::vector<int> GetActiveLaserModels() const;

private:

	std::vector<LaserInfo> lasers_;

	VECTOR worldPos_;

	int spawnTimer_ = 0; // 次のレーザー生成用タイマー
	int activeCount_ = 0; // 今何本目か

	void InitLoad(void) override;
	void InitTransform(void) override;
	void InitPost(void) override;


	void UpdateWave1(void) override;
	void UpdateWave2(void) override;
	void UpdateWave3(void) override;
};