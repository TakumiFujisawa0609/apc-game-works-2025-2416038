#include "StageBase.h"

StageBase::StageBase(void)
{
}

StageBase::~StageBase(void)
{
}

void StageBase::Init(int baseModel)
{
	// モデル読み込み
	modelId_ = MV1DuplicateModel(baseModel);
    // 衝突判定情報の構築
    MV1SetupCollInfo(modelId_, -1);

	//img_ = LoadGraph("Data/Image/タイトルなし.png");
	InitTransform();

	InitTransformPost();

}

void StageBase::Update(void)
{
}

void StageBase::Draw(void)
{
	DrawGraph(0, 0, img_, true);

	// ステージ描画
	MV1DrawModel(modelId_);

    //// モデルの上に少し浮かせて描く
    //const float lineY = pos_.y + 38.0f;

    //// 範囲
    //const float left = -800.0f;
    //const float right = 800.0f;
    //const float front = -800.0f; // 手前
    //const float back = 800.0f; // 奥
    //const int divisions = 3;

    //unsigned int color = GetColor(255, 0, 0); // 赤線

    //// 区切り間隔
    //float stepX = (right - left) / divisions; // 約266.66
    //float stepZ = (back - front) / divisions;

    //// 横線（X方向に伸びる）
    //for (int i = 0; i <= divisions; ++i)
    //{
    //    float z = front + stepZ * i;
    //    VECTOR a = VGet(left, lineY, z);
    //    VECTOR b = VGet(right, lineY, z);
    //    DrawLine3D(a, b, color);  
    //}

    //// 縦線（Z方向に伸びる）
    //for (int i = 0; i <= divisions; ++i)
    //{
    //    float x = left + stepX * i;
    //    VECTOR a = VGet(x, lineY, front);
    //    VECTOR b = VGet(x, lineY, back);
    //    DrawLine3D(a, b, color);
    //}
}

void StageBase::Release(void)
{
	// モデル解放
	MV1DeleteModel(modelId_);

	DeleteGraph(img_);
}

void StageBase::SetPos(VECTOR pos)
{
	pos_ = pos;
	MV1SetPosition(modelId_, pos_);
}

int StageBase::GetModelId()
{
	return modelId_;
}

void StageBase::InitTransformPost(void)
{
	MV1SetPosition(modelId_, pos_);

	MV1SetScale(modelId_, scales_);
}
