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

	//img_ = LoadGraph("Data/Image/タイトルなし.png")
}

void StageBase::Update(void)
{
}

void StageBase::Draw(void)
{
	DrawGraph(0, 0, img_, true);

	// ステージ描画
	MV1DrawModel(modelId_);
}

void StageBase::Release(void)
{
    // モデルのコリジョン情報を破棄
    MV1TerminateCollInfo(modelId_, -1);
	// モデル解放
	MV1DeleteModel(modelId_);

	DeleteGraph(img_);
}

void StageBase::SetPos(VECTOR pos)
{
	pos_ = pos;
	MV1SetPosition(modelId_, pos_);
}

void StageBase::SetScales(VECTOR scales)
{
	scales_ = scales;
	MV1SetScale(modelId_, scales_);
}

void StageBase::SetRot(VECTOR rot)
{
	rot_ = rot;
	MV1SetRotationXYZ(modelId_, rot_);
}

int StageBase::GetModelId()
{
	return modelId_;
}