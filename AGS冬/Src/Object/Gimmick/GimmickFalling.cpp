#include <algorithm>
#include "GimmickFalling.h"

GimmickFalling::GimmickFalling() {}
GimmickFalling::~GimmickFalling() {}

void GimmickFalling::SetupWave() 
{
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
        info.hasLanded = false;
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
            if (info.warningTimer >= 120.0f) { // 2.0秒で落下
                info.isWarning = false;
                info.isFalling = true;
                info.modelHandle = MV1DuplicateModel(modelId_);

                int r = GetRand(3);            // 0,1,2,3 のどれか
                float rotY = (float)r * DX_PI_F * 0.5f;
                info.rotY = rotY;              // WaveInfo に rotY を追加する必要あり

                MV1SetRotationXYZ(info.modelHandle, VGet(0.0f, rotY, 0.0f));
                MV1SetPosition(info.modelHandle, VGet(info.panelPos.x, info.fallY, info.panelPos.z));
            }
        }

        // 落下
        if (info.isFalling) {
            info.fallY -= 15.0f;
            if (info.fallY <= -10.0f) {
                info.fallY = -10.0f;
                info.isFalling = false;
                info.hasLanded = true;
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
            float t = info.warningTimer / 120.0f;
            if (t > 1.0f) t = 1.0f;
            float radius = 290.0f * t;
            int col = GetColor(255, 0, 0);
            VECTOR top = VGet(info.panelPos.x, 5.0f, info.panelPos.z);
            VECTOR bottom = VGet(info.panelPos.x, 0.0f, info.panelPos.z);
            DrawCone3D(top, bottom, radius, 32, col, col, TRUE);
        }
		MV1DrawModel(info.modelHandle);
    }

}

void GimmickFalling::Release() {
    for (auto& info : waveInfos_)
        if (info.modelHandle >= 0) MV1DeleteModel(info.modelHandle);
    MV1DeleteModel(modelId_);
}

TYPE GimmickFalling::GetType() const
{
    return TYPE::FALLING;
}

std::vector<VECTOR> GimmickFalling::GetPositions() const
{
    std::vector<VECTOR> positions;

    for (auto info : waveInfos_) {
        // 落下中またはまだ残っているオブジェクトを対象にする
        if (info.isFalling || (!info.isWarning && info.alpha > 0)) {
            positions.push_back(VGet(info.panelPos.x, info.fallY, info.panelPos.z));
        }
    }

    return positions;
}

bool GimmickFalling::HasLanded() const
{
    for (const auto& info : waveInfos_) {
        if (info.hasLanded)
            return true;
    }
    return false;
}

std::vector<int> GimmickFalling::GetActiveModelIds() const
{
    std::vector<int> ids;

    for (const auto& info : waveInfos_) {
        // モデルが有効（生成済みで、地面についてない）なら追加
        if (info.modelHandle >= 0 && info.hasLanded == false) {
            ids.push_back(info.modelHandle);
        }
    }

    return ids;
}

void GimmickFalling::InitLoad()
{
}

void GimmickFalling::InitTransform()
{
	
}

void GimmickFalling::InitPost()
{
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

void GimmickFalling::UpdateWave1(void)
{
}

void GimmickFalling::UpdateWave2(void)
{
}

void GimmickFalling::UpdateWave3(void)
{
}
