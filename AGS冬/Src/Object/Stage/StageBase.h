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

protected:

	// ステージモデルのハンドルID
	int modelId_;
	// ステージモデルの位置
	VECTOR pos_;
};
