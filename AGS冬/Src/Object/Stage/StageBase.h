#pragma once
#include <DxLib.h>
class StageBase
{
public:

	enum class TYPE
	{
		PANEL,
		MAX,
	};

	StageBase(void);
	virtual ~StageBase(void);

	void Init(int baseModel);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	void SetPos(VECTOR pos);

	int GetModelId();

protected:

	// ステージモデルのハンドルID
	int modelId_;
	// ステージモデルの位置
	VECTOR pos_;
	VECTOR scales_;

	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;

	// 大きさ、回転、座標のモデル設定
	void InitTransformPost(void);
};
