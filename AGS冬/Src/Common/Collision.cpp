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
    VECTOR down = VGet(playerPos.x, playerPos.y - 30.0f, playerPos.z);
    float r = 30.0f;

    MV1_COLL_RESULT_POLY result;

    if (stageManager_->IsCollisionLine(top, down, &result))
    {
        // プレイヤーに衝突座標を渡す
        player_->CollisionStage(result.HitPosition);
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
