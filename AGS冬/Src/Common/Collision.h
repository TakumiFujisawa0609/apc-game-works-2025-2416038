#pragma once
#include <DxLib.h>
class Player;
class StageManager;

class Collision
{
public:

	// AABB構造体
	struct AABB 
	{
		VECTOR min;  // 最小座標 (左下奥)
		VECTOR max;  // 最大座標 (右上手前)
	};

	Collision();
	~Collision();

	void Init(Player* player, StageManager* stageManager);
	void Update();
	void Draw();
	void Release();



private:

	Player* player_;
	StageManager* stageManager_;

	// 衝突判定(床)
	void PlayerAndFloorCollision();
};

