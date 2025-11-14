#pragma once
#include <string>
#include <DxLib.h>
#include "../../Utility/CollisionShape.h"
class AnimationController;

class ActorBase
{
public:
	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 20.0f;
	// コンストラクタ
	ActorBase(void);
	// デストラクタ
	virtual ~ActorBase(void);

	void Init(void);
	void Load(void);			// 読み込み
	void LoadEnd(void);			// 読み込み後
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	// 座標を取得
	const VECTOR& GetPos(void) const;

	virtual CollisionShape::AABB GetAABB() const;

	const VECTOR& GetRot() const;

protected:

	// アニメーション制御
	AnimationController* animationController_;

	// アニメーション種別
	int animType_;

	// モデル情報
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	VECTOR localAngles_;

	// 移動方向
	VECTOR moveDir_;

	// カプセルの当たり判定座標
	VECTOR startCapsulePos_;
	VECTOR endCapsulePos_;
	float capsuleRadius_;

	// 横幅・縦幅
	float halfW_;
	float halfH_;

	int cntDamage_ = 0;

	int hitStopTimer_ = 0;  // ヒットストップ中フレーム数

	// リソースロード
	virtual void InitLoad(void) = 0;
	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;
	// 大きさ、回転、座標のモデル設定
	void InitTransformPost(void);
	// アニメーションの初期化
	virtual void InitAnimation(void) = 0;
	// 初期化後の個別処理
	virtual void InitPost(void) = 0;

	// 遅延回転処理
	void DelayRotate(void);
};