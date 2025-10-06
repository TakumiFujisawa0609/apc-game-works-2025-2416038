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
}

void Player::Draw(void)
{
	ActorBase::Draw();

	DrawFormatString(
		0, 50, 0xffffff,
		"キャラ角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);
}

void Player::Release(void)
{
	ActorBase::Release();
}

void Player::Move(void)
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
	const float movePow = 6.0f;

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
		// 停止したら待機アニメーション再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	}


}

void Player::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");
}

void Player::InitTransform(void)
{
	// 大きさ
	scales_ = { 1.0f, 1.0f, 1.0f };

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// 位置
	pos_ = AsoUtility::VECTOR_ZERO;

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

}