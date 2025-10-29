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
		RUN,
		JUMP,
		FALL,
		DEAD,
		END,
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
		DEAD,
		FALL,
		IDLE,
		JUMP,
		RUN,
		MAX,
	};

	// 重力
	static constexpr float GRAVITY_POW = 0.8f;
	// ジャンプ力
	static constexpr float JUMP_POW = 30.0f;

	// 地面衝突用線分の長さ
	static constexpr float COLLISION_LEN = 10.0f;

	// リスポーン開始位置
	static constexpr float RESPAWN_LEN = -3500.0f;

	// リスポーン位置
	static constexpr VECTOR RESPAWN_POS = { 0.0f, 550.0f, 0.0f };

	// 標準の拡散光色
	static constexpr COLOR_F COLOR_DIF_DEFAULT = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 点滅時の拡散光色
	static constexpr COLOR_F COLOR_DIF_BLINK = { 1.0f, 0.5f, 0.5f, 1.0f };
	// 点滅間隔
	static constexpr int TERM_BLINK = 8;

	// 最大HP
	static constexpr int MAX_HP = 3;

	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// 状態遷移
	void ChangeState(STATE state);

	// 無敵中(ダメージ、ノックバックを受けない)
	bool IsInvincible(void);

	// リスポーン処理
	void Respawn();

	// 終了遷移
	bool IsStateEnd(void);

	// ダメージを与える
	void Damage(int damage);
	// HPの取得
	int GetHp(void);

	// カプセルの当たり判定を取得
	const VECTOR& GetStartCapsulePos(void) const { return startCapsulePos_; }
	const VECTOR& GetEndCapsulePos(void) const { return endCapsulePos_; }
	const float& GetCapsuleRadius(void) const { return capsuleRadius_; }


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
	void ChanegeRun(void);
	void ChangeJump(void);
	void ChabgeFall(void);
	void ChangeDead(void);
	void ChangeEnd(void);

	// 状態別更新
	void UpdateIdle(void);
	void UpdateRun(void);
	void UpdateJump(void);
	void UpdateFall(void);
	void UpdateDead(void);
	void UpdateEnd(void);

	// 状態別描画
	void DrawIdle(void);
	void DrawRun(void);
	void DrawJump(void);
	void DrawFall(void);
	void DrawDead(void);
	void DrawEnd(void);

	bool isJump_;
	float jumpPow_;

	int hp_;

	bool isRespawn_ = true;

private:
};

