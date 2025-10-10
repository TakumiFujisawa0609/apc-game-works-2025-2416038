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

	//InitTransform();

	InitTransformPost();
}

void StageBase::Update(void)
{
}

void StageBase::Draw(void)
{
	// ステージ描画
	MV1DrawModel(modelId_);
}

void StageBase::Release(void)
{
	// モデル解放
	MV1DeleteModel(modelId_);
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
	pos_ = { 0.0f, -39.22f, 0.0f };

	scales_ = { 5.0f, 1.0f, 5.0f };

	MV1SetPosition(modelId_, pos_);

	MV1SetScale(modelId_, scales_);
}
