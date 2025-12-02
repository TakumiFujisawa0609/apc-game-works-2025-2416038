#include "../../Manager/SceneManager.h"
#include "GimmickBase.h"

GimmickBase::GimmickBase(void)
{

}

GimmickBase::~GimmickBase(void)
{
}

void GimmickBase::Init(int baseModel)
{
	// ÉÇÉfÉãì«Ç›çûÇ›
	modelId_ = MV1DuplicateModel(baseModel);

	InitPost();
}

void GimmickBase::Update(void)
{
	
}

void GimmickBase::Draw(void)
{
}

void GimmickBase::Release(void)
{
	isActive_ = false;
}

bool GimmickBase::IsActive()
{
	return isActive_;
}

TYPE GimmickBase::GetType() const
{
	return TYPE::NONE;
}

VECTOR GimmickBase::GetPos() const
{
	return VGet(0.0f, -400.0f, 0.0f);
}

int GimmickBase::GetModelId() const
{
	return modelId_;
}

std::vector<VECTOR> GimmickBase::GetPositions() const
{
	return { VGet(0.0f, -400.0f, 0.0f) };
}

void GimmickBase::DrawWave1(void)
{
}

void GimmickBase::DrawWave2(void)
{
}

void GimmickBase::DrawWave3(void)
{
}

void GimmickBase::InitTransformPost(void)
{
	
}