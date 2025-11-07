#include "GimmickLaser.h"

GimmickLaser::GimmickLaser(void) {}
GimmickLaser::~GimmickLaser(void) {}

void GimmickLaser::Init()
{
    GimmickBase::Init();
    lasers_.clear();
    spawnTimer_ = 0;
    activeCount_ = 0;

    worldPos_ = { 0,0,0 };

    // モデルロード（共通レーザーモデル）
    modelId_ = MV1LoadModel("Data/Model/Gimmick/Laser.mv1");

    // マテリアルをすべてエミッシブに設定
    int materialNum = MV1GetMaterialNum(modelId_);
    for (int i = 0; i < materialNum; ++i)
    {
        MV1SetMaterialDifColor(modelId_, i, GetColorF(1, 1, 1, 1)); // 拡散反射
        MV1SetMaterialSpcColor(modelId_, i, GetColorF(1, 1, 1, 1)); // 鏡面反射
        MV1SetMaterialEmiColor(modelId_, i, GetColorF(1, 1, 1, 1)); // 自発光
        MV1SetMaterialAmbColor(modelId_, i, GetColorF(1, 1, 1, 1)); // 環境光
    }
}

void GimmickLaser::Update()
{
    if (!isActive_) return;

    spawnTimer_++;

    // 4秒(240F)ごとに1本追加、最大4本
    if (spawnTimer_ >= 240 && activeCount_ <= 4)
    {
        spawnTimer_ = 0;

        LaserInfo l;
        l.direction = GetRand(3);
        l.timer = 0;
        l.fired = true;
        l.pos = 1200.0f;    // スタート位置

        // 個別モデルを作成（複製）
        l.modelHandle = MV1DuplicateModel(modelId_);

        lasers_.push_back(l);
        activeCount_++;
    }

    for (auto& l : lasers_)
    {
        if (!l.fired) continue;

        l.timer++;
        const float speed = 10.0f;

        // 移動
        switch (l.direction)
        {
        case 0: l.pos -= speed; break; // 左→右
        case 1: l.pos -= speed; break; // 右→左
        case 2: l.pos -= speed; break; // 奥→手前
        case 3: l.pos -= speed; break; // 手前→奥
        }

        // モデルの位置と回転を設定
        VECTOR pos;
        float rotY = 0.0f;

        switch (l.direction)
        {
        case 0: pos = VGet(-l.pos, 40.0f, 300.0f); rotY = DX_PI_F * 0.5f; break; // 左→右
        case 1: pos = VGet(l.pos, 40.0f, 300.0f);  rotY = DX_PI_F * -0.5f; break; // 右→左
        case 2: pos = VGet(300.0f, 40.0f, -l.pos); rotY = 0.0f; break; // 奥→手前
        case 3: pos = VGet(300.0f, 40.0f, l.pos);  rotY = DX_PI_F; break; // 手前→奥
        }

        // ワールド座標を保存！
        worldPos_ = pos;

        MV1SetPosition(l.modelHandle, pos);
        MV1SetRotationXYZ(l.modelHandle, VGet(0, rotY, 0));

        // サイズ
        MV1SetScale(l.modelHandle, VGet(3.0f, 1.0f, 1.0f));

        // 消滅条件
        if (l.pos < -1200.0f)
        {
            MV1DeleteModel(l.modelHandle);
            l.fired = false;
        }
    }

    // 4本打ったら終了
    if (activeCount_ == 5)
        isActive_ = false;
}

void GimmickLaser::Draw()
{
    if (!isActive_) return;

    for (auto& l : lasers_)
    {
        if (!l.fired) continue;
        MV1DrawModel(l.modelHandle);
    }
}

void GimmickLaser::Release()
{
    for (auto & l : lasers_)
    {
        if (l.fired)
            MV1DeleteModel(l.modelHandle);
    }

    lasers_.clear();

    // 元モデル削除
    MV1DeleteModel(modelId_);
}

GimmickType GimmickLaser::GetType() const
{
    return GimmickType::LASER;
}

VECTOR GimmickLaser::GetPos() const
{
    return worldPos_;
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
