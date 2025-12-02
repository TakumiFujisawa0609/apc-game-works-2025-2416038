#include "../Object/Gimmick/GimmickLaser.h"
#include "../Object/Gimmick/GimmickFalling.h"
#include "../Object/Gimmick/GimmickBase.h"
#include "../Manager/SceneManager.h"
#include "GimmickManager.h"

GimmickManager::GimmickManager()
{
}

GimmickManager::~GimmickManager()
{
}

void GimmickManager::Init()
{
	// ギミックモデルの読み込み
	gimmickModelIds_.emplace_back(
		MV1LoadModel("Data/Model/Gimmick/Laser.mv1")); // レーザー
	gimmickModelIds_.emplace_back(
		MV1LoadModel("Data/Model/Gimmick/100kg.mv1")); // 落下物

    // 生成
	GimmickBase* lazer = new GimmickLaser();
    //lazer->Init(gimmickModelIds_[static_cast<int>(TYPE::LASER)]);
    // レーザーをギミックにを登録
	gimmicks_.emplace_back(lazer);

	// 生成
	GimmickBase* falling = new GimmickFalling();
    //falling->Init(gimmickModelIds_[static_cast<int>(TYPE::FALLING)]);
	// 落下物をギミックに登録
    gimmicks_.emplace_back(falling);


    
	// 画像読み込み
	img100kg_ = LoadGraph("Data/Image/100kg.png");
	imgLazer_ = LoadGraph("Data/Image/lazer.png");

    timer_ = 0;
    currentGimmick_ = nullptr;

    // 最初のギミック待ち用タイマー
    firstDelayTimer_ = 0;
    firstDelayDuration_ = 200;

	// ウェーブ・ウェーブタイマー初期化
    ChangeWave(WAVE::WAVE1);
    waveTimer_ = 0.0f;

}

void GimmickManager::Update()
{
	// ウェーブタイマー更新
    waveTimer_ += SceneManager::GetInstance()->GetDeltaTime();
    // 60秒経過ごとにウェーブ（難易度）変更
    if (waveTimer_ >= 60) ChangeWave(WAVE::WAVE2);
    else if (waveTimer_ >= 120) ChangeWave(WAVE::WAVE3);

    // gimmicks_ が空なら一切処理できない
    if (gimmicks_.empty()) return;

    // --- ギミックが無いなら新しいのを開始 ---
    if (currentGimmick_ == nullptr)
    {
        firstDelayTimer_++;

        if (firstDelayTimer_ >= firstDelayDuration_)
        {
			// ランダムでギミック選択
            int index = GetRand((int)gimmicks_.size() - 1);
            currentGimmick_ = gimmicks_[index];
            currentGimmick_->Init(gimmickModelIds_[index]);
        }
        return;
    }

    // 現在のギミックがアクティブなら更新
    if (currentGimmick_->IsActive()) {
        switch (wave_)
        {
        case WAVE::WAVE1:
            currentGimmick_->UpdateWave1();
            break;
        case WAVE::WAVE2:
            currentGimmick_->UpdateWave2();
            break;
        case WAVE::WAVE3:
            currentGimmick_->UpdateWave3();
            break;
        }
        return;
    }

    // ギミックが終了したら次のギミックをランダム選択
    int index = GetRand((int)gimmicks_.size() - 1);
    currentGimmick_ = gimmicks_[index];
    currentGimmick_->Init(gimmickModelIds_[index]);
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
        case TYPE::LASER: 
            gimmickName = "レーザー";
			DrawModiBillboard3D({ 300.0f, 600.0f, 2200.0f },
				-300.0f,330.0f, // 左上
				300.0f, 330.0f,  // 右上
				300.0f, -330.0f, // 右下
				-300.0f, -330.0f,// 左下
                imgLazer_, TRUE);
            break;
        case TYPE::FALLING:
            gimmickName = "落下物";
            DrawModiBillboard3D({ 300.0f, 600.0f, 2200.0f },
                -300.0f, 330.0f, // 左上
                300.0f, 330.0f,  // 右上
                300.0f, -330.0f, // 右下
                -300.0f, -330.0f,// 左下
                img100kg_, TRUE);
            break;
        case TYPE::QUIZ: 
            gimmickName = "クイズ";
            break;
        default: break;
        }

        // 現在のギミックがアクティブなら更新
        if (currentGimmick_->IsActive()) {
            switch (wave_)
            {
            case WAVE::WAVE1:
                currentGimmick_->DrawWave1();
                break;
            case WAVE::WAVE2:
                currentGimmick_->DrawWave2();
                break;
            case WAVE::WAVE3:
                currentGimmick_->DrawWave3();
                break;
            }
        }

            
		SetFontSize(50);
       // DrawFormatString(680, 80, GetColor(255, 255, 0), "発動中 : %s", gimmickName);
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
    if (!currentGimmick_ || currentGimmick_->GetType() != TYPE::LASER) {
        return 0; // NULLならクラッシュを避け、無効なIDを返す
    }
    return currentGimmick_->GetModelId();
}

VECTOR GimmickManager::GetLaserPos()
{
    if (!currentGimmick_ || currentGimmick_->GetType() != TYPE::LASER)
        return VGet(0, 1000, 0);

    return currentGimmick_->GetPos();
}

std::vector<VECTOR> GimmickManager::GetFallingPos()
{
    if (!currentGimmick_ || currentGimmick_->GetType() != TYPE::FALLING)
        return {};

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

void GimmickManager::ChangeWave(WAVE wave)
{
    wave_ = wave;

    switch (wave_)
    {
    case WAVE::WAVE1:
        break;
    case WAVE::WAVE2:
        break;
    case WAVE::WAVE3:
        break;
    }
}

//GimmickBase& GimmickManager::GetGimmicks()
//{
//    return currentGimmick_;
//}
