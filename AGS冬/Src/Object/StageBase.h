#pragma once
#include <DxLib.h>
class StageBase
{
public:
	StageBase(void);
	~StageBase(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	// ステージモデルのハンドルID
	int modelId_;
	// ステージモデルの位置
	VECTOR pos_;
};
