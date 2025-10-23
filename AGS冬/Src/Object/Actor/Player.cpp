#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Common/AnimationController.h"
#include "../../Utility/MatrixUtility.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Update(void)
{
	ActorBase::Update();

	switch (state_)
	{
	case STATE::IDLE:
		UpdateIdle();
		break;
	case STATE::DEAD:
		UpdateDead();
		break;
	case STATE::END:
		UpdateEnd();
		break;
	}

	// リスポーン処理
	Respawn();
}

void Player::Draw(void)
{
	ActorBase::Draw();

	switch (state_)
	{
	case STATE::IDLE:
		DrawIdle();
		break;
	case STATE::DEAD:
		DrawDead();
		break;
	case STATE::END:
		DrawEnd();
		break;
	}

	VECTOR p = VAdd(pos_, VGet(0.0f, 80.0f, 0.0f));
	//DrawSphere3D(p, 40.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
	/*DrawFormatString(
		0, 50, 0xffffff,
		"キャラ角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);*/

	DrawFormatString(0,0,0xffffff,"JumpState:%d",jumpState_);
}

void Player::Release(void)
{
	ActorBase::Release();
}

void Player::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case STATE::IDLE:
		ChangeIdle();
		break;
	case STATE::DEAD:
		ChangeDead();
		break;
	case STATE::END:
		ChangeEnd();
		break;
	}
}

bool Player::IsInvincible(void)
{
	return false;
}

void Player::Respawn()
{
	if (!isRespawn_) return; // 死んだら処理しない

	if (pos_.y < RESPAWN_LEN)
	{
		// 落下前に生きているかチェック
		if (hp_ <= 1) // ダメージで0になる場合
		{
			Damage(1);       // HP減らす
			ChangeState(STATE::DEAD); // 即死亡
			return;          // リスポーン処理中断
		}

		// まだ生きているならリスポーン
		pos_ = RESPAWN_POS;
		jumpPow_ = 0.0f;
		jumpState_ = JumpState::Falling;

		Damage(1);
	}
	MV1SetPosition(modelId_, pos_);
}

bool Player::IsStateEnd(void)
{
	return state_ == STATE::DEAD;
}

void Player::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ < 0)
	{
		hp_ = 0;
	}

	// hpが０になったら死亡状態に
	if (hp_ == 0) {
		ChangeState(STATE::DEAD);
	}
}

int Player::GetHp(void)
{
	return hp_;
}

void Player::CollisionStage(VECTOR pos)
{
	//printfDx("Collision hit pos: (%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);

	//if (jumpState_ == JumpState::Rising) return; // 上昇中は無視

	// 衝突判定に指定座標に押し戻す
	//  少し上に押し戻すことでガクつき防止
	pos_.y = pos.y;
	jumpPow_ = 0.0f;
	jumpState_ = JumpState::Ground;

	// モデル位置反映
	//MV1SetPosition(modelId_, pos_);
}

void Player::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// カメラの角度を取得
	VECTOR cameraAngles_ = SceneManager::GetInstance()->GetCamera()->GetAngles();

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// ゲームパッドが接続数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// WASDで移動する
		if (ins.IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
		if (ins.IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; }
		if (ins.IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; }
		if (ins.IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; }
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
		dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

	}

	// WASDでカメラを移動させる
	const float movePow = 8.0f;

	if (!AsoUtility::EqualsVZero(dir))
	{
		// XYZの回転行列
		// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(cameraAngles_.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// 移動方向から角度に変換
		//angles_.y = atan2f(moveDir_.x, moveDir_.z);

		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir_, movePow));

		// 歩行アニメーション再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);
	}
	else
	{
		// 待機アニメーション
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}


}

// ジャンプ処理
void Player::ProcessJump(void)
{
	InputManager& ins = InputManager::GetInstance();

	// ゲームパッド入力取得
	InputManager::JOYPAD_IN_STATE pad =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// --- ジャンプ入力判定 ---
	bool isJumpPressed =
		ins.IsTrgDown(KEY_INPUT_SPACE) ||                  // キーボード SPACE
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, // ゲームパッド Aボタン
			InputManager::JOYPAD_BTN::A);

	// --- ジャンプ開始処理 ---
	if (isJumpPressed && jumpState_ == JumpState::Ground)
	{
		jumpState_ = JumpState::Rising;
		jumpPow_ = JUMP_POW;

		// ジャンプアニメーション再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
	}

	// 重力(加速度を速度に加算していく)
	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;

	// 落下中なら、空中にいると見なす
	if (jumpPow_ < 0.0f)
	{
		jumpState_ = JumpState::Falling;
	}
}

void Player::ChangeIdle(void)
{
	// アニメ再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void Player::ChangeJump(void)
{
	// アニメ再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP));
}

void Player::ChangeDead(void)
{
	// アニメ再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEAD));

	isRespawn_ = false;
}

void Player::ChangeEnd(void)
{
}

void Player::UpdateIdle(void)
{
	// 移動処理
	ProcessMove();
	// ジャンプ処理
	ProcessJump();
}

void Player::UpdateJump(void)
{
}

void Player::UpdateDead(void)
{
}

void Player::UpdateEnd(void)
{
}

void Player::DrawIdle(void)
{
}

void Player::DrawJump(void)
{
}

void Player::DrawDead(void)
{
}

void Player::DrawEnd(void)
{
}

void Player::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");
}

void Player::InitTransform(void)
{
	// 大きさ
	scales_ = { 0.8f, 0.8f, 0.8f };

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// 位置
	pos_ = VGet(150.0f, 300.0f, 150.0f);

	// 当たり判定を作成
	startCapsulePos_ = { 0.0f,110,0.0f };
	endCapsulePos_ = { 0.0f,15.0f,0.0f };
	capsuleRadius_ = 20.0f;
}

void Player::InitAnimation(void)
{
	state_ = STATE::IDLE;

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	// 外部アニメーションを登録
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f, "Data/Model/Player/Idle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f, "Data/Model/Player/Walk.mv1");

	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}


void Player::InitPost(void)
{
	jumpState_ = JumpState::Ground;

	// ジャンプ力の初期化
	jumpPow_ = 0.0f;
	// ジャンプフラグ初期化
	isJump_ = false;

	halfW_ = 30.0f;
	halfH_ = 80.0f;

	hp_ = MAX_HP;
}