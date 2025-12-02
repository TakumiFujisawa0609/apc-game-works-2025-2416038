#pragma once
#include <vector>
#include <DxLib.h>

enum class TYPE {
	NONE,
	LASER,	 // レーザー
	FALLING, // 落下物
	QUIZ,	 // クイズ
	MAX
};

class GimmickBase
{
public:
	GimmickBase(void);
	virtual ~GimmickBase(void);

	virtual void Init(int baseModel);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	bool IsActive();

	virtual TYPE GetType() const;

	virtual VECTOR GetPos() const;

	virtual int GetModelId() const;

	virtual std::vector<VECTOR> GetPositions() const;

	virtual void UpdateWave1(void) = 0;
	virtual void UpdateWave2(void) = 0;
	virtual void UpdateWave3(void) = 0;

	virtual void DrawWave1(void);
	virtual void DrawWave2(void);
	virtual void DrawWave3(void);

protected:
	
	int modelId_;

	bool isActive_ = false;  // 現在発動中かどうか

	VECTOR scl_;

	// 画像
	int gimmickImg_;

	// リソースロード
	virtual void InitLoad(void) = 0;
	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;
	// 大きさ、回転、座標のモデル設定
	void InitTransformPost(void);

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;
};