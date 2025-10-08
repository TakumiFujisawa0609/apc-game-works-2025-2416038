#include "../Object/Actor/Player.h"
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
	// ステージブロックとプレイヤーの衝突
    VECTOR playerPos = player_->GetPos();
    VECTOR top = VGet(playerPos.x, playerPos.y, playerPos.z);
    VECTOR down = VGet(playerPos.x, playerPos.y - 100.0f, playerPos.z);

    MV1_COLL_RESULT_POLY result;

    const auto& stages = stageManager_->GetStages();
    for (const auto pair : stages)
    {
        for (StageBase* stage : pair.second)
        {
            int stageModelId = stage->GetModelId();

            result = MV1CollCheck_Line(stageModelId, -1, top, down, -1);
            // 当たってたら
            if (result.HitFlag == 1)
            {
                VECTOR hitPos = result.HitPosition;
                hitPos.y += 1.0f; // 少し上に押し戻して浮き沈み防止

                player_->CollisionStage(hitPos);

                return; // 最初のヒットだけ処理
            }
        }
    }
}

void Collision::Draw()
{
}

void Collision::Release()
{
}

// AABB同士の判定関数
bool Collision::IsHitAABB(const AABB& a, const AABB& b)
{
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
			(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
			(a.min.z <= b.max.z && a.max.z >= b.min.z);
}
