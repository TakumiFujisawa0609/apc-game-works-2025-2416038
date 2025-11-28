#pragma once
#include <DxLib.h>

class StageBase
{
public:

	enum class TYPE
	{
		PANEL,
		MONITOR,
		MAX,
	};

	StageBase(void);
	virtual ~StageBase(void);

	void Init(int baseModel);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	void SetPos(VECTOR pos);
	void SetScales(VECTOR scales);
	void SetRot(VECTOR rot);

	int GetModelId();

protected:
	int img_;
	// ステージモデルのハンドルID
	int modelId_;
	// ステージモデルの位置
	VECTOR pos_;
	VECTOR scales_;
	VECTOR rot_;

	// 色の初期化
	virtual void InitColor(void) = 0;
};
