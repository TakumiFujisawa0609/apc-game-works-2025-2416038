#include "Panel.h"

Panel::Panel(void)
{
}

Panel::~Panel(void)
{
}

void Panel::InitTransform(void)
{
	//pos_ = { 0.0f, 0.0f, 0.0f };

	scales_ = { 1.0f, 1.0f, 1.0f };
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
