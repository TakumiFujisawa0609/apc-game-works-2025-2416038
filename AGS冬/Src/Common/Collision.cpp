#include "../Object/Actor/Player.h"
#include "../Object/Stage/StageBase.h"
#include "../Manager/GimmickManager.h"
#include "../Manager/StageManager.h"
#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::Init(Player* player, StageManager* stageManager, GimmickManager* gimmickManager)
{
	player_ = player;
	stageManager_ = stageManager;
	gimmickManager_ = gimmickManager;
}

void Collision::Update()
{
	PlayerAndFloorCollision();

	PlayerAndLaser();

	PlayerAndFallingObjects();
}

void Collision::Draw()
{
	// 座標を所得
	VECTOR pPos = player_->GetPos();

	// 前フレーム位置 → 現在位置 にレイを伸ばす
	VECTOR startPos = VAdd(pPos, { 0,100.0f, 0 });
	VECTOR endPos = VAdd(pPos, { 0, 0.0f, 0 });

	//DrawLine3D(startPos, endPos, GetColor(255, 0, 0));

	VECTOR sPos = VAdd(pPos, { 0, 80, 0 });
	VECTOR ePos = VAdd(pPos, { 0, 25, 0 });


	//DrawCapsule3D(sPos, ePos, 25, 10, 0xff0000, 0xff0000, false);

	//DrawSphere3D(sPos, 40, 10, 0xff0000, 0xff0000, false);
}

void Collision::Release()
{
}

void Collision::PlayerAndFloorCollision()
{
	// 座標を所得
	VECTOR pPos = player_->GetPos();

	// レイを伸ばす
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
			// モデルのコリジョン情報を構築
			MV1SetupCollInfo(modelId, -1);

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
	/*const auto& gimmicks = gimmickManager_->GetGimmicks();
	if (gimmicks.GetType() == TYPE::LASER) return;*/

	// プレイヤーが無敵だったら早期リターン
	if (player_->IsInvincible()) {
		return;
	}

	// 座標を所得
	VECTOR pPos = player_->GetPos();
	// カプセルの始点終点
	VECTOR sPos = VAdd(pPos, { 0, 80, 0 });
	VECTOR ePos = VAdd(pPos, { 0, 25, 0 });

	// モデルID
	auto laserIds = gimmickManager_->GetLaserModelIds();
	for (int modelId : laserIds)
	{
		// モデルのコリジョン情報を構築
		MV1SetupCollInfo(modelId, -1);

		// カプセルとモデルの衝突判定
		MV1_COLL_RESULT_POLY_DIM res =
			MV1CollCheck_Capsule(modelId, -1, sPos, ePos, 20, -1);

		if (res.HitNum > 0)
		{
			player_->Damage(1);
		}
	}
}

void Collision::PlayerAndFallingObjects()
{
	// プレイヤーが無敵だったら早期リターン
	if (player_->IsInvincible()) return;

	// 座標を所得
	VECTOR pPos = player_->GetPos();
	// カプセルの始点終点
	VECTOR sPos = VAdd(pPos, { 0, 80, 0 });
	VECTOR ePos = VAdd(pPos, { 0, 25, 0 });

	// モデルID
	auto fallingObjectIds = gimmickManager_->GetFallingObjectModelIds();

	for (int modelId : fallingObjectIds)
	{
		// モデルのコリジョン情報を構築
		MV1SetupCollInfo(modelId, -1);

		// カプセルとモデルの衝突判定
		MV1_COLL_RESULT_POLY_DIM res =
			MV1CollCheck_Capsule(modelId, -1, sPos, ePos, 20, -1);

		if (res.HitNum > 0)
		{
			int a = 1;
			player_->Damage(1);
		}
	}
}
