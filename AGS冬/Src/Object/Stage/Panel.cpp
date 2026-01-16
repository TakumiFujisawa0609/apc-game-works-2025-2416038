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

     //マテリアルをすべてエミッシブに設定
    int materialNum = MV1GetMaterialNum(modelId_);
    for (int i = 0; i < materialNum; ++i)
    {
        MV1SetMaterialDifColor(modelId_, i, GetColorF(0.5f, 0.5f, 0.5f, 1)); // 拡散反射
        MV1SetMaterialSpcColor(modelId_, i, GetColorF(0.5f, 0.5f, 0.5f, 1)); // 鏡面反射
        MV1SetMaterialEmiColor(modelId_, i, GetColorF(0.4f, 0.4f, 0.4f, 1.0f)); // 自発光
        MV1SetMaterialAmbColor(modelId_, i, GetColorF(1, 1, 1, 1)); // 環境光
    }
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
