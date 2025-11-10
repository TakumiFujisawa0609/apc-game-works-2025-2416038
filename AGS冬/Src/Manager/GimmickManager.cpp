#include "../Object/Gimmick/GimmickLaser.h"
#include "../Object/Gimmick/GimmickFalling.h"
#include "../Object/Gimmick/GimmickBase.h"
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

        DrawFormatString(0, 300, 0xffffff,
            "Laser座標　 ：(% .1f, % .1f, % .1f)",
            GetLaserPos().x, GetLaserPos().y, GetLaserPos().z);

        // --- 落下物座標（複数ある）---
        auto fallingPositions = GetFallingPos();
        int yOffset = 340;
        for (size_t i = 0; i < fallingPositions.size(); ++i) {
            const auto& p = fallingPositions[i];
            DrawFormatString(0, yOffset, GetColor(0, 255, 0),
                "落下物[%zu] ：(% .1f, % .1f, % .1f)", i, p.x, p.y, p.z);
            yOffset += 20;
        }
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

const std::map<GimmickBase::TYPE, std::vector<GimmickBase*>>& GimmickManager::GetGimmicks()
{
    return currentGimmick_;
}

VECTOR GimmickManager::GetLaserPos()
{
    return currentGimmick_->GetPos();
}

std::vector<VECTOR> GimmickManager::GetFallingPos()
{
    return currentGimmick_->GetPositions();
}
