#pragma once
#include <DxLib.h>
#include "ActorBase.h"

class Player : public ActorBase
{
public:

	// 状態
	enum class STATE
	{
		IDLE,
		DEAD,
		MAX,
	};

	enum class JumpState {
		Ground,
		Rising,
		Falling,
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		JUMP,
		DEAD,
		MAX,
	};

	// 重力
	static constexpr float GRAVITY_POW = 1.0f;
	// ジャンプ力
	static constexpr float JUMP_POW = 30.0f;

	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// 状態遷移
	void ChangeState(STATE state);

	void CollisionStage(VECTOR pos);

protected:

	STATE state_;
	JumpState jumpState_;

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

	// 移動処理
	void ProcessMove(void);
	void ProcessJump(void);

	// 状態遷移
	void ChangeIdle(void);
	void ChangeJump(void);
	void ChangeDead(void);

	// 状態別更新
	void UpdateIdle(void);
	void UpdateJump(void);
	void UpdateDead(void);

	// 状態別描画
	void DrawIdle(void);
	void DrawJump(void);
	void DrawDead(void);

	bool isJump_;
	float jumpPow_;

private:
};

