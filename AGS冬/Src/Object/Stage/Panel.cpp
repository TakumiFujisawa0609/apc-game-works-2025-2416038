#include "Panel.h"

Panel::Panel(void)
{
}

Panel::~Panel(void)
{
}

void Panel::InitColor(void)
{
	// エミッシブカラー設定
	//MV1SetMaterialEmiColor(modelId_, 0, GetColorF()); // 縁
}

void Panel::Update(void)
{
	StageBase::Update();
}

void Panel::Draw(void)
{
	StageBase::Draw();
}

void Panel::Release(void)
{
	StageBase::Release();
}
