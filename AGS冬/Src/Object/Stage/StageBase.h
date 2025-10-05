#pragma once
#include <DxLib.h>
class StageBase
{
public:

	StageBase(void);
	virtual ~StageBase(void);

	void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

protected:

	// ステージモデルのハンドルID
	int modelId_;
	// ステージモデルの位置
	VECTOR pos_;
};
