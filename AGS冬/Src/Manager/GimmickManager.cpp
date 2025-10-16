#include "../Object/Gimmick/GimmickLaser.h"
#include "GimmickManager.h"

GimmickManager::GimmickManager()
{
}

GimmickManager::~GimmickManager()
{
}

void GimmickManager::Init()
{
    gimmicks_.push_back(new GimmickLaser());
    timer_ = 0;
    currentGimmick_ = nullptr;
}

void GimmickManager::Update()
{
    if (currentGimmick_ && currentGimmick_->IsActive()) {
        currentGimmick_->Update();
        return;
    }

    // 一定時間ごとに新しいギミックを発動
    timer_++;
    if (timer_ > 300) {
        timer_ = 0;
        int index = GetRand((int)gimmicks_.size() - 1);
        currentGimmick_ = gimmicks_[index];
        currentGimmick_->Init();
    }
}

void GimmickManager::Draw()
{
    if (currentGimmick_) currentGimmick_->Draw();
}

void GimmickManager::Release()
{
    for (auto g : gimmicks_) {
        g->Release();
        delete g;
    }

    gimmicks_.clear();
}
