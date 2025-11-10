#include "GimmickBase.h"

GimmickBase::GimmickBase(void)
{
}

GimmickBase::~GimmickBase(void)
{
}

void GimmickBase::Init(void)
{
	isActive_ = true;
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

GimmickType GimmickBase::GetType() const
{
	return GimmickType::NONE;
}

VECTOR GimmickBase::GetPos() const
{
	return VGet(0.0f, -400.0f, 0.0f);
}

int GimmickBase::modelId() const
{
	return modelId_;
}

std::vector<VECTOR> GimmickBase::GetPositions() const
{
	return { VGet(0.0f, -400.0f, 0.0f) };
}

void GimmickBase::InitTransformPost(void)
{
	
}
