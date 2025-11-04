#include "../Object/Actor/Player.h"
#include "../Object/Stage/StageBase.h"
#include "../Manager/StageManager.h"
#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::Init(Player* player, StageManager* stageManager)
{
	player_ = player;
	stageManager_ = stageManager;
}

void Collision::Update()
{
	PlayerAndFloorCollision();
}

void Collision::Draw()
{
	// 座標を所得
	VECTOR pPos = player_->GetPos();

	// 前フレーム位置 → 現在位置 にレイを伸ばす
	VECTOR startPos = VAdd(pPos, { 0,100.0f, 0 });
	VECTOR endPos = VAdd(pPos, { 0, 0.0f, 0 });

	DrawLine3D(startPos, endPos, GetColor(255, 0, 0));
}

void Collision::Release()
{
}

void Collision::PlayerAndFloorCollision()
{
	// 座標を所得
	VECTOR pPos = player_->GetPos();

	// 前フレーム位置 → 現在位置 にレイを伸ばす
	VECTOR startPos = VAdd(pPos, { 0,100.0f, 0 });
	VECTOR endPos = VAdd(pPos, { 0, 0.0f, 0 });

	// ステージ全体に対してチェック
	const auto& stages = stageManager_->GetStages();
	for (auto pair : stages)
	{
		for (StageBase* stage : pair.second)
		{
			// ステージのモデルを取得
			int modelId = stage->GetModelId();

			// 線分とモデルの衝突判定
			MV1_COLL_RESULT_POLY res =
				MV1CollCheck_Line(modelId, -1, startPos, endPos);

			if (res.HitFlag)
			{
				// 当たった位置
				VECTOR hitPos = res.HitPosition;

				// プレイヤーに衝突情報を渡す
				player_->CollisionStage(hitPos);

				return;
			}
		}
	}
}

void Collision::PlayerAndLaser()
{
	// 座標を所得
	VECTOR pPos = player_->GetPos();


}
