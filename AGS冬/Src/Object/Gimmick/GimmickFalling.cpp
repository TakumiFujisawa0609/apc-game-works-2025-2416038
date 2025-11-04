#include <algorithm>
#include "GimmickFalling.h"

GimmickFalling::GimmickFalling() {}
GimmickFalling::~GimmickFalling() {}

void GimmickFalling::Init() {

    GimmickBase::Init();

    modelId_ = MV1LoadModel("Data/Model/Gimmick/100kg.mv1");

    // 3x3 パネル座標を Gimmick 内で作る
    panels_.clear();
    const float startX = -300.0f;
    const float startZ = -300.0f;
    const float step = 600.0f;
    for (int z = 0; z < 3; ++z) {
        for (int x = 0; x < 3; ++x) {
            panels_.push_back(VGet(startX + x * step, 0.0f, startZ + z * step));
        }
    }

    currentWave_ = 0;
    waveTimer_ = 0.0f;
    waveActive_ = false;
    waveInfos_.clear();

    scl_ = { 1.45f, 1.45f, 1.45f };

    SetupWave();
}

void GimmickFalling::SetupWave() {
    waveInfos_.clear();
    std::vector<int> usedIndices;

    for (int i = 0; i < 3; ++i) { // 1ウェーブで3パネル選択
        int idx;
        do {
            idx = GetRand((int)panels_.size() - 1);
        } while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end());
        usedIndices.push_back(idx);

        WaveInfo info;
        info.panelPos = panels_[idx];
        info.warningTimer = 0.0f;
        info.isWarning = true;
        info.isFalling = false;
        info.fallY = 1000.0f;
        info.modelHandle = -1;
        info.alpha = 255;
        waveInfos_.push_back(info);
    }

    waveActive_ = true;
    waveTimer_ = 0.0f;
}

void GimmickFalling::Update() {
    if (!isActive_ || !waveActive_) return;

    waveTimer_ += 1.0f;
    bool allFinished = true;

    for (auto& info : waveInfos_) {
        // 警告
        if (info.isWarning) {
            info.warningTimer += 1.0f;
            if (info.warningTimer >= 180.0f) { // 3.0秒で落下
                info.isWarning = false;
                info.isFalling = true;
                info.modelHandle = MV1DuplicateModel(modelId_);

                MV1SetPosition(info.modelHandle, VGet(info.panelPos.x, info.fallY, info.panelPos.z));
            }
        }

        // 落下
        if (info.isFalling) {
            info.fallY -= 15.0f;
            if (info.fallY <= 0.0f) {
                info.fallY = 0.0f;
                info.isFalling = false;
            }
            MV1SetScale(info.modelHandle, scl_);
            MV1SetPosition(info.modelHandle, VGet(info.panelPos.x, info.fallY, info.panelPos.z));
        }

        // フェードアウト
        if (!info.isWarning && !info.isFalling && info.modelHandle >= 0) {
            info.alpha -= 5;
            if (info.alpha <= 0) {
                MV1DeleteModel(info.modelHandle);
                info.modelHandle = -1;
                info.alpha = 0;
            }
        }

        if (info.alpha > 0) allFinished = false;
    }

    if (allFinished) {
        currentWave_++;
        waveActive_ = false;
        if (currentWave_ < 3) SetupWave();
        else isActive_ = false;
    }
}

void GimmickFalling::Draw() {
    if (!isActive_) return;

    for (auto& info : waveInfos_) {
        // 警告コーン
        if (info.isWarning) {
            float t = info.warningTimer / 180.0f;
            if (t > 1.0f) t = 1.0f;
            float radius = 290.0f * t;
            int col = GetColor(255, 0, 0);
            VECTOR top = VGet(info.panelPos.x, 5.0f, info.panelPos.z);
            VECTOR bottom = VGet(info.panelPos.x, 0.0f, info.panelPos.z);
            DrawCone3D(top, bottom, radius, 32, col, col, TRUE);
        }
		MV1DrawModel(info.modelHandle);

		DrawFormatString(0, 500, 0xffffff, "pos(%.f, %.f, %.f)", info.panelPos.x, info.fallY, info.panelPos.z);
    }
 /*   for (auto& info : waveInfos_) {
        if (info.isWarning && info.warningTimer == 1.0f)
            printfDx("パネル(%.0f, %.0f) : 警告開始\n", info.panelPos.x, info.panelPos.z);

        if (info.isFalling && info.fallY == 1000.0f)
            printfDx("パネル(%.0f, %.0f) : 落下開始\n", info.panelPos.x, info.panelPos.z);

        if (!info.isWarning && !info.isFalling && info.alpha == 250)
            printfDx("パネル(%.0f, %.0f) : フェードアウト開始\n", info.panelPos.x, info.panelPos.z);
    }*/
}

void GimmickFalling::Release() {
    for (auto& info : waveInfos_)
        if (info.modelHandle >= 0) MV1DeleteModel(info.modelHandle);
    MV1DeleteModel(modelId_);
}

GimmickType GimmickFalling::GetType() const { return GimmickType::FALLING; }

void GimmickFalling::InitLoad()
{
}

void GimmickFalling::InitTransform()
{
	
}

void GimmickFalling::InitPost()
{
}