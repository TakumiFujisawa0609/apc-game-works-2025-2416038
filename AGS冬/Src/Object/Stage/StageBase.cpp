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

	// 位置
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);
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
