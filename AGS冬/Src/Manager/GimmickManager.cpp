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

    // 最初のギミック待ち用タイマー
    firstDelayTimer_ = 0;
    firstDelayDuration_ = 200;
}

void GimmickManager::Update()
{
    // --- ギミックが無いなら新しいのを開始 ---
    if (currentGimmick_ == nullptr)
    {
        firstDelayTimer_++;

        if (firstDelayTimer_ >= firstDelayDuration_)
        {
            int index = GetRand((int)gimmicks_.size() - 1);
            currentGimmick_ = gimmicks_[index];
            currentGimmick_->Init();
        }
        return;
    }

    if (currentGimmick_ && currentGimmick_->IsActive()) {
        currentGimmick_->Update();
        return;
    }

    // ギミックが終了したら次を開始
     int index = GetRand((int)gimmicks_.size() - 1);
     currentGimmick_ = gimmicks_[index];
     currentGimmick_->Init();

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
        case TYPE::LASER: gimmickName = "レーザー"; break;
        case TYPE::FALLING:  gimmickName = "落下物"; break;
        case TYPE::QUIZ:   gimmickName = "クイズ"; break;
        default: break;
        }
            
		SetFontSize(50);
        DrawFormatString(680, 80, GetColor(255, 255, 0), "発動中 : %s", gimmickName);
        SetFontSize(16);

        //DrawFormatString(0, 300, 0xffffff,
        //    "Laser座標　 ：(% .1f, % .1f, % .1f)",
        //    GetLaserPos().x, GetLaserPos().y, GetLaserPos().z);

        //// --- 落下物座標（複数ある）---
        //auto fallingPositions = GetFallingPos();
        //int yOffset = 340;
        //for (size_t i = 0; i < fallingPositions.size(); ++i) {
        //    const auto& p = fallingPositions[i];
        //    DrawFormatString(0, yOffset, GetColor(0, 255, 0),
        //        "落下物[%zu] ：(% .1f, % .1f, % .1f)", i, p.x, p.y, p.z);
        //    yOffset += 20;
        //}
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

int GimmickManager::GetModelId() const
{
    // currentGimmick_が有効なギミックを指しているかチェック！
    if (currentGimmick_ == nullptr || currentGimmick_->GetType() != TYPE::LASER) {
        return 0; // NULLならクラッシュを避け、無効なIDを返す
    }
    return currentGimmick_->GetModelId();
}

VECTOR GimmickManager::GetLaserPos()
{
    return currentGimmick_->GetPos();
}

std::vector<VECTOR> GimmickManager::GetFallingPos()
{
    return currentGimmick_->GetPositions();
}

std::vector<int> GimmickManager::GetLaserModelIds() const
{
    std::vector<int> ids;

    if (currentGimmick_ && currentGimmick_->GetType() == TYPE::LASER) {
        const auto* laser = dynamic_cast<GimmickLaser*>(currentGimmick_);
        if (laser) {
            ids = laser->GetActiveLaserModels();
        }
    }
    return ids;
}

std::vector<int> GimmickManager::GetFallingObjectModelIds() const
{
    std::vector<int> ids;

    if (currentGimmick_ && currentGimmick_->GetType() == TYPE::FALLING) {
        const auto* falling = dynamic_cast<GimmickFalling*>(currentGimmick_);
        if (falling) return falling->GetActiveModelIds();
    }

    return ids;
}

//GimmickBase& GimmickManager::GetGimmicks()
//{
//    return currentGimmick_;
//}
