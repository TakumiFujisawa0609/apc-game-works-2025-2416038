#include "GimmickLaser.h"

GimmickLaser::GimmickLaser(void) {}
GimmickLaser::~GimmickLaser(void) {}

void GimmickLaser::Update()
{
	GimmickBase::Update();
}

void GimmickLaser::Draw()
{
    if (!isActive_) return;

    for (auto& l : lasers_)
    {
        if (!l.fired) continue;
        MV1DrawModel(l.modelHandle);
    }

   

  //  // 3Dビルボード
  //  DrawBillboard3D(
  //      { 300.0f, 800.0f, 2600.0f }, // 座標
  //      0.5, 0.5f, // x,yの中心座標
  //      1300.0f,   // サイズ
		//0.0f,     // 回転角度
  //      gimmickImg_, // 画像ハンドル
		//true,       // 透過処理
		//false, false    // x,yの反転
  //  );
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

TYPE GimmickLaser::GetType() const
{
    return TYPE::LASER;
}

VECTOR GimmickLaser::GetPos() const
{
    return worldPos_;
}

std::vector<int> GimmickLaser::GetActiveLaserModels() const
{
    std::vector<int> handles;
    for (const auto& l : lasers_) {
        if (l.fired) {
            handles.push_back(l.modelHandle);
        }
    }
    return handles;
}


void GimmickLaser::InitLoad(void)
{
    gimmickImg_ = LoadGraph("Data/Image/Gimmick/lazer.png");
}

void GimmickLaser::InitTransform(void)
{
}

void GimmickLaser::InitPost(void)
{
    lasers_.clear();
    spawnTimer_ = 0;
    activeCount_ = 0;

    worldPos_ = { 0,0,0 };

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

void GimmickLaser::UpdateWave1(void)
{
    if (!isActive_) return;

    spawnTimer_++;

    // 4秒(240F)ごとに1本追加、最大4本
    if (spawnTimer_ >= 120 && activeCount_ <= 8)
    {
        spawnTimer_ = 0;

        LaserInfo l;
        l.direction = GetRand(3);
        l.timer = 0;
        l.fired = true;
        l.pos = 1200.0f;    // スタート位置

        // 高さをランダムに決定（40 または 140）
        l.height = (GetRand(1) == 0) ? 40.0f : 140.0f;

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
        case 0: pos = VGet(-l.pos, l.height, 300.0f); rotY = DX_PI_F * 0.5f; break; // 左→右
        case 1: pos = VGet(l.pos, l.height, 300.0f);  rotY = DX_PI_F * -0.5f; break; // 右→左
        case 2: pos = VGet(300.0f, l.height, -l.pos); rotY = 0.0f; break; // 奥→手前
        case 3: pos = VGet(300.0f, l.height, l.pos);  rotY = DX_PI_F; break; // 手前→奥
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
    if (activeCount_ == 8)
        isActive_ = false;
}

void GimmickLaser::UpdateWave2(void)
{
    if (!isActive_) return;

    spawnTimer_++;

    // 4秒(240F)ごとに1本追加、最大4本
    if (spawnTimer_ >= 120 && activeCount_ <= 8)
    {
        spawnTimer_ = 0;

        LaserInfo l;
        l.direction = GetRand(3);
        l.timer = 0;
        l.fired = true;
        l.pos = 1200.0f;    // スタート位置

        // 高さをランダムに決定（40 または 140）
        l.height = (GetRand(1) == 0) ? 40.0f : 140.0f;

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
        case 0: pos = VGet(-l.pos, l.height, 300.0f); rotY = DX_PI_F * 0.5f; break; // 左→右
        case 1: pos = VGet(l.pos, l.height, 300.0f);  rotY = DX_PI_F * -0.5f; break; // 右→左
        case 2: pos = VGet(300.0f, l.height, -l.pos); rotY = 0.0f; break; // 奥→手前
        case 3: pos = VGet(300.0f, l.height, l.pos);  rotY = DX_PI_F; break; // 手前→奥
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
    if (activeCount_ == 8)
        isActive_ = false;
}

void GimmickLaser::UpdateWave3(void)
{
    if (!isActive_) return;

    spawnTimer_++;

    // 4秒(240F)ごとに1本追加、最大4本
    if (spawnTimer_ >= 120 && activeCount_ <= 8)
    {
        spawnTimer_ = 0;

        LaserInfo l;
        l.direction = GetRand(3);
        l.timer = 0;
        l.fired = true;
        l.pos = 1200.0f;    // スタート位置

        // 高さをランダムに決定（40 または 140）
        l.height = (GetRand(1) == 0) ? 40.0f : 140.0f;

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
        case 0: pos = VGet(-l.pos, l.height, 300.0f); rotY = DX_PI_F * 0.5f; break; // 左→右
        case 1: pos = VGet(l.pos, l.height, 300.0f);  rotY = DX_PI_F * -0.5f; break; // 右→左
        case 2: pos = VGet(300.0f, l.height, -l.pos); rotY = 0.0f; break; // 奥→手前
        case 3: pos = VGet(300.0f, l.height, l.pos);  rotY = DX_PI_F; break; // 手前→奥
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
    if (activeCount_ == 8)
        isActive_ = false;
}
