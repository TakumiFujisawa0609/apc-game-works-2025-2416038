#pragma once
#include <vector>
#include <DxLib.h>

enum class TYPE {
	NONE,
	LASER,	 // レーザー
	FALLING, // 落下物
	QUIZ,	 // クイズ
	MAX
};;

class GimmickBase
{
public:
	GimmickBase(void);
	virtual ~GimmickBase(void);

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	bool IsActive();

	virtual TYPE GetType() const;

	virtual VECTOR GetPos() const;

	virtual int GetModelId() const;

	virtual std::vector<VECTOR> GetPositions() const;

protected:
	
	int modelId_;

	bool isActive_ = false;  // 現在発動中かどうか

	VECTOR scl_;

	// リソースロード
	virtual void InitLoad(void) = 0;
	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;
	// 大きさ、回転、座標のモデル設定
	void InitTransformPost(void);

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;
};