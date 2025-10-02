#include "Player.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Manager/SceneManager.h"
#include "../Common/AnimationController.h"
#include "../Utility/MatrixUtility.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");

	// 状態
	animType_ = ANIM_TYPE::IDLE;


	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };

	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	// 位置
	pos_ = AsoUtility::VECTOR_ZERO;
	MV1SetPosition(modelId_, pos_);

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	// 外部アニメーションを登録
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f, "Data/Model/Player/Idle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f, "Data/Model/Player/Walk.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::THROW), 30.0f, "Data/Model/Player/Throw.mv1");

	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	// サイコロ初期化
	InitDice();

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };
}

void Player::Update(void)
{
	// 行列の合成　(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	// 移動処理
	Move();
	MV1SetPosition(modelId_, pos_);

	// プレイヤーの遅延回転処理
	DelayRotate();

	// アニメーションの更新
	animationController_->Update();

	// サイコロのZ軸回転
	diceAngles_.z += 0.01f;

	//SyncDice();
}

void Player::Draw(void)
{
	// モデル描画
	MV1DrawModel(modelId_);

	// サイコロ描画
	MV1DrawModel(diceModelId_);

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
	// モデル解放
	MV1DeleteModel(modelId_);

	// サイコロ解放
	MV1DeleteModel(diceModelId_);

	// アニメーションコントローラの解放
	animationController_->Release();
	delete animationController_;
}

VECTOR Player::GetPos(void)
{
	return pos_;
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
	const float movePow = 3.0f;

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

		//// 回転行列を使用して、ベクトルを回転させる
		//moveDir_ = VTransform(dir, mat);
		//// 移動方向から角度に変換する
		////angles_.y = atan2f(moveDir_.x, moveDir_.z);
		//// 方向×スピードで移動量を作って、座標に足して移動
		//pos_ = VAdd(pos_, VScale(moveDir_, MOVE_POW));

	}


}

void Player::DelayRotate(void)
{
	// 移動方向から角度に変換する
	float goal = atan2f(moveDir_.x, moveDir_.z);
	// 常に最短経路で補間
	angles_.y = AsoUtility::LerpAngle(angles_.y, goal, 0.2f);
}

void Player::InitDice(void)
{
	// サイコロ読み込み
	diceModelId_ = MV1LoadModel("Data/Model/Dice.mv1");
	diceScales_ = { 0.1f, 0.1f, 0.1f };
	diceLocalPos_ = { 200.0f, 100.f, 0.0f };

	// 位置
	MV1SetPosition(diceModelId_, VAdd(pos_, diceLocalPos_));
	// 大きさ
	MV1SetScale(diceModelId_, diceScales_);
}

// サイコロの位置・回転同期
void Player::SyncDice(void)
{
	// プレイヤーとの相対位置同期
	//Playerの回転に合わせて、Playerからサイコロまでの相対座標を回転
	diceLocalPos_ = VTransform({ 200.0f, 100.f, 0.0f }, MatrixUtility::GetMatrixRotateXYZ(angles_));

	// サイコロの位置
	MV1SetPosition(diceModelId_, VAdd(pos_, diceLocalPos_));

	// サイコロの回転
	MV1SetRotationMatrix(diceModelId_, MatrixUtility::GetMatrixRotateXYZ(diceAngles_));
}
