#include "../Object/Gimmick/GimmickLaser.h"
#include "../Object/Gimmick/GimmickFalling.h"
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
    gimmicks_.push_back(new GimmickFalling());

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
    if (currentGimmick_)
    {
        currentGimmick_->Draw();

        // --- デバッグ表示 ---
        const char* gimmickName = "None";
        switch (currentGimmick_->GetType())
        {
        case GimmickType::LASER: gimmickName = "レーザー"; break;
        case GimmickType::FALLING:  gimmickName = "落下物"; break;
        case GimmickType::QUIZ:   gimmickName = "クイズ"; break;
        default: break;
        }
        
		SetFontSize(50);
        DrawFormatString(640, 80, GetColor(255, 255, 0), "発動中 : %s", gimmickName);
        SetFontSize(16);
    }
}

void GimmickManager::Release()
{
    for (auto g : gimmicks_) {
        g->Release();
        delete g;
    }

    gimmicks_.clear();
}
