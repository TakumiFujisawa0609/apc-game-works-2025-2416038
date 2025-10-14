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

	//img_ = LoadGraph("Data/Image/タイトルなし.png");
	//InitTransform();

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
	pos_ = { 0.0f, -39.22f, 0.0f };

	scales_ = { 2.0f, 1.0f, 2.0f };

	MV1SetPosition(modelId_, pos_);

	MV1SetScale(modelId_, scales_);
}
