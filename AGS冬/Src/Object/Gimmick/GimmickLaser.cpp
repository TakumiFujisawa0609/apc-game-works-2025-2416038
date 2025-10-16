#include "GimmickLaser.h"

GimmickLaser::GimmickLaser(void) {}
GimmickLaser::~GimmickLaser(void) {}

void GimmickLaser::Init()
{
    GimmickBase::Init();
    lasers_.clear();
    spawnTimer_ = 0;
    activeCount_ = 0;
    isActive_ = true;
}

void GimmickLaser::Update()
{
    if (!isActive_) return;

    spawnTimer_++;

    // 2秒(120F)ごとに1本追加、最大4本
    if (spawnTimer_ >= 120 && activeCount_ < 4)
    {
        spawnTimer_ = 0;

        LaserInfo l;
        l.direction = GetRand(3);
        l.timer = 0;
        l.fired = true;
        l.pos = 1000.0f; // スタート位置（遠方から）

        lasers_.push_back(l);
        activeCount_++;
    }
     
    bool allEnd = true;

    for (auto& l : lasers_)
    {
        if (!l.fired) continue;
        allEnd = false;

        l.timer++;

        // 移動スピード
        const float speed = 25.0f;

        // 進行方向ごとの移動
        switch (l.direction)
        {
        case 0: l.pos -= speed; break; // 左→右
        case 1: l.pos -= speed; break; // 右→左
        case 2: l.pos -= speed; break; // 奥→手前
        case 3: l.pos -= speed; break; // 手前→奥
        }

        // ある程度近づいたら消滅
        if (l.pos < -1000.0f)
            l.fired = false;
    }

    // 全部消えたら終了
    if (activeCount_ >= 4 && allEnd)
        isActive_ = false;
}

void GimmickLaser::Draw()
{
    if (!isActive_) return;

    unsigned int color = GetColor(255, 0, 0);
    float len = 800.0f;
    float y = 40.0f;

    for (const auto& l : lasers_)
    {
        if (!l.fired) continue;

        VECTOR start, end;

        switch (l.direction)
        {
        case 0: // 左→右
            start = VGet(-l.pos, y, -len);
            end = VGet(-l.pos, y, len);
            break;

        case 1: // 右→左
            start = VGet(l.pos, y, -len);
            end = VGet(l.pos, y, len);
            break;

        case 2: // 奥→手前
            start = VGet(-len, y, -l.pos);
            end = VGet(len, y, -l.pos);
            break;

        case 3: // 手前→奥
            start = VGet(-len, y, l.pos);
            end = VGet(len, y, l.pos);
            break;
        }

        DrawLine3D(start, end, color);
    }
}

void GimmickLaser::Release()
{
    lasers_.clear();
}

void GimmickLaser::InitLoad(void)
{
}

void GimmickLaser::InitTransform(void)
{
}

void GimmickLaser::InitPost(void)
{
}
