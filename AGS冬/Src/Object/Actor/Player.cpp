#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
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
	// --- 演出専用の更新（点滅など） ---
	if (state_ == STATE::DAMAGE)
	{
		cntDamage_++;
	}
	
	// --- ヒットストップ中は移動や入力を止める ---
	if (hitStopTimer_ > 0)
	{
		hitStopTimer_--;
		return;
	}

	ActorBase::Update();

	switch (state_)
	{
	case STATE::STANBY:
		UpdateStanby();
		break;
	case STATE::DAMAGE:
		UpdateDamage();
		break;
	case STATE::FALL:
		UpdateFall();
		break;
	case STATE::DEAD:
		UpdateDead();
		break;
	case STATE::END:
		UpdateEnd();
		break;
	case STATE::VICTORY:
		UpdateVictory();
		break;
	}

	// アニメーションの更新
	animationController_->Update();

	auto& ins = InputManager::GetInstance();
//	if (ins.IsTrgDown(KEY_INPUT_1)) Damage(1);
	// リスポーン処理
	Respawn();
}

void Player::Draw(void)
{
	switch (state_)
	{
	case STATE::STANBY:
		DrawStanby();
		break;
	case STATE::DAMAGE:
		DrawDamage();
		break;
	case STATE::FALL:
		DrawFall();
		break;
	case STATE::DEAD:
		DrawDead();
		break;
	case STATE::END:
		DrawEnd();
		break;
	case STATE::VICTORY:
		DrawVictory();
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
	/*DrawFormatString(
		600, 0, 0xffffff,
		"キャラ座標　 ：(% .1f, % .1f, % .1f)",
		pos_.x,
		pos_.y,
		pos_.z
	);*/
	//DrawFormatString(500,0,0xffffff,"state:%d",state_);

	ActorBase::Draw();
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
	case STATE::STANBY:
		ChangeStanby();
		break;
	case STATE::DAMAGE:
		ChangeDamage();
		break;
	case STATE::FALL:
		ChabgeFall();
		break;
	case STATE::DEAD:
		ChangeDead();
		break;
	case STATE::END:
		ChangeEnd();
		break;
	case STATE::VICTORY:
		ChangeVictory();
		break;
	}
}

bool Player::IsInvincible(void)
{
	return state_ == STATE::DAMAGE
		|| state_ == STATE::DEAD
		|| state_ == STATE::END;
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

bool Player::IsStateDead(void)
{
	return state_ == STATE::DEAD;
}

bool Player::IsStateEnd(void)
{
	return state_ == STATE::END;
}

void Player::Damage(int damage)
{
	hp_ -= damage;

	cntDamage_ = 0;	// ダメージカウンター初期化

	hitStopTimer_ = 50;	// プレイヤーだけヒットストップ開始

	ChangeState(STATE::DAMAGE);

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
	// 衝突判定時に指定座標に押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;
	jumpState_ = JumpState::Ground;
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
	const float movePow = 5.0f;

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
		if (jumpState_ == JumpState::Ground) 
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::RUN), true);
			// 足音再生
			SoundManager::GetInstance()->PlayWalk();
		}
	}
	else
	{
		// 歩行音停止
		SoundManager::GetInstance()->StopWalk();
		// 地上のみアイドルアニメ
		if (jumpState_ == JumpState::Ground)
			animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
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

	// 空中でも移動可能
	ProcessMove();

	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;

	if (pos_.y < -1.0f)
	{
		jumpState_ = JumpState::Falling;
	}

	if (jumpPow_ < 0.0f && jumpState_ != JumpState::Ground)
	{
		jumpState_ = JumpState::Falling;
		animationController_->Play(static_cast<int>(ANIM_TYPE::FALL), true);
	}
}

void Player::ChangeStanby(void)
{
}

void Player::ChangeDamage(void)
{
}

void Player::ChabgeFall(void)
{

}

void Player::ChangeDead(void)
{
	// アニメ再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEAD), false);

	isRespawn_ = false;
}

void Player::ChangeEnd(void)
{
}

void Player::ChangeVictory(void)
{
	// 勝利アニメーション
	animationController_->Play(static_cast<int>(ANIM_TYPE::VICTORY), true);
}

void Player::UpdateStanby(void)
{
	// 移動処理
	ProcessMove();
	// ジャンプ処理
	ProcessJump();
}

void Player::UpdateDamage(void)
{
	if (cntDamage_ >= 120)
	{
		ChangeState(STATE::STANBY);
	}

	// 移動処理
	ProcessMove();
	// ジャンプ処理
	ProcessJump();
}

void Player::UpdateFall(void)
{
}

void Player::UpdateDead(void)
{
	// 撃破アニメーションが終わったら、END状態にする
	if (animationController_->IsEnd()) {
		ChangeState(STATE::END);
	}
}

void Player::UpdateEnd(void)
{
}

void Player::UpdateVictory(void)
{
	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;

	if (pos_.y <= 0)
	{
		pos_.y = 0;
	}
}

void Player::DrawStanby(void)
{
}

void Player::DrawDamage(void)
{
	// 点滅周期：5フレームごとに切り替え
	bool visible = ((cntDamage_ / 11) % 2 == 0);
	float alpha = visible ? 1.0f : 0.3f; // 半透明時は0.3

	int materialNum = MV1GetMaterialNum(modelId_);
	for (int i = 0; i < materialNum; ++i)
	{
		// 現在のマテリアル色を取得して、αだけ変更
		COLOR_F dif = MV1GetMaterialDifColor(modelId_, i);
		dif.a = alpha;
		MV1SetMaterialDifColor(modelId_, i, dif);
	}

}

void Player::DrawFall(void)
{
}

void Player::DrawDead(void)
{
}

void Player::DrawEnd(void)
{
}

void Player::DrawVictory(void)
{
}

void Player::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel("Data/Model/Player/Mouse.mv1");
}

void Player::InitTransform(void)
{
	// 大きさ
	scales_ = { 0.8f, 0.8f, 0.8f };

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// 位置
	pos_ = VGet(150.0f, 0.0f, 150.0f);

	// 当たり判定を作成
	startCapsulePos_ = { 0.0f,110,0.0f };
	endCapsulePos_ = { 0.0f,15.0f,0.0f };
	capsuleRadius_ = 20.0f;

	// マテリアルをすべてエミッシブに設定
	int materialNum = MV1GetMaterialNum(modelId_);
	for (int i = 0; i < materialNum; ++i)
	{
		MV1SetMaterialDifColor(modelId_, i, GetColorF(1.0f, 1.0f, 1.0f, 1)); // 拡散反射
		MV1SetMaterialSpcColor(modelId_, i, GetColorF(1.0f, 1.0f, 1.0f, 0.5f)); // 鏡面反射
		MV1SetMaterialEmiColor(modelId_, i, GetColorF(1.0f, 1.0f, 1.0f, 0.1f)); // 自発光
		MV1SetMaterialAmbColor(modelId_, i, GetColorF(1.0f, 1.0f, 1.0f, 0.5f)); // 環境光
	}
}

void Player::InitAnimation(void)
{
	state_ = STATE::STANBY;

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	// 外部アニメーションを登録
	//animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f, "Data/Model/Player/Idle.mv1");
	//animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f, "Data/Model/Player/Walk.mv1");

	 // FBX内部アニメを登録
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::IDLE), 60.0f, 2);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::RUN), 60.0f, 4);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::JUMP), 60.0f, 3);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::FALL), 60.0f, 1);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::DEAD), 60.0f, 0);

	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::VICTORY), 60.0f, 5);

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