#pragma once
#include <DxLib.h>
class Player;
class StageManager;
class GimmickManager;

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

	void Init(Player* player, StageManager* stageManager, GimmickManager* gimmickManager);
	void Update();
	void Draw();
	void Release();

private:

	Player* player_;
	StageManager* stageManager_;
	GimmickManager* gimmickManager_;

	// プレイヤーと床
	void PlayerAndFloorCollision();

	// プレイヤーとレーザー
	void PlayerAndLaser();

	// プレイヤーと落下物
	void PlayerAndFallingObjects();
};

