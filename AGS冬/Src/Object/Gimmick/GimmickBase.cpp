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

void GimmickBase::InitTransformPost(void)
{
}
