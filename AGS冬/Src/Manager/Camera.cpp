#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/Player.h"
#include "Camera.h"

Camera::Camera(void)
{
	// DxLibの初期設定では、
	// カメラの位置が x = 320.0f, y = 240.0f, z = (画面のサイズによって変化)、
	// 注視点の位置は x = 320.0f, y = 240.0f, z = 1.0f
	// カメラの上方向は x = 0.0f, y = 1.0f, z = 0.0f
	// 右上位置からZ軸のプラス方向を見るようなカメラ
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期位置
	pos_ = DERFAULT_POS;

	// カメラの初期角度
	angles_ = DERFAULT_ANGLES;
}

void Camera::Update(void)
{
	// ズーム補間
	if (fabs(zoomDistance_ - zoomTarget_) > 0.001f)
	{
		zoomDistance_ += (zoomTarget_ - zoomDistance_) * zoomSpeed_;
	}
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	if (!follow_) return;

	// プレイヤー座標と回転
	VECTOR playerPos = follow_->GetPos();
	float playerRotY = follow_->GetRot().y;

	// プレイヤー正面方向のベクトル
	VECTOR forward = VGet(sinf(playerRotY), 0.0f, cosf(playerRotY));

	// カメラ位置：プレイヤーの正面の逆方向 + 高さ
	float distance = 500.0f * zoomDistance_; // 距離調整
	float height = 150.0f;                  // 高さ調整
	VECTOR offset = VScale(forward, distance);
	offset.y += height;
	pos_ = VAdd(playerPos, offset);

	// 注視点はプレイヤー
	targetPos_ = playerPos;

	// 上方向ベクトル
	VECTOR up = VGet(0.0f, 1.0f, 0.0f);

	// カメラ角度を自動計算
	VECTOR dir = VSub(targetPos_, pos_);
	angles_.y = atan2f(dir.x, dir.z);
	angles_.x = -atan2f(dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));

	// カメラ設定
	SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, up);
}
void Camera::SetBeforeDrawFree(void)
{
	// ゲームパッドが接続されているかで処理を分ける
	if (GetJoypadNum() == 0)
	{
		// 方向回転によるXYZの移動
		MoveXYZDirection();
	}
	else
	{
		// 方向回転によるXYZの移動(ゲームパッド)
		MoveXYZDirectionPad();
	}

	// 注視点の設定
	VECTOR followPos = follow_->GetPos();
	targetPos_ = followPos;
}

void Camera::DrawDebug(void)
{
	DrawFormatString(
		0, 10, 0xffffff,
		"カメラ座標　 ：(% .1f, % .1f, % .1f)",
		pos_.x, pos_.y, pos_.z
	);
	DrawFormatString(
		0, 30, 0xffffff,
		"カメラ角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);

	DrawSphere3D(targetPos_, 20.0f, 10, 0xff0000, 0xff0000, true);
}

void Camera::SetBeforeDrawFollow(void)
{

	auto& ins = InputManager::GetInstance();
	if (GetJoypadNum() == 0)
	{
		// 方向回転によるXYZの移動
		MoveXYZDirection();
	}
	else
	{
		// 方向回転によるXYZの移動(ゲームパッド)
		MoveXYZDirectionPad();
	}

	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));
	//mat = MMult(mat, MGetRotZ(angles_.z));

	// 注視点の移動
	VECTOR followPos = follow_->GetPos();
	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	targetPos_ = VAdd(followPos, targetLocalRotPos);
	// カメラの移動
	// 相対座標を回転させて、回転後の相対座標を取得する
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);

	// ズームを適用
	cameraLocalRotPos = VScale(cameraLocalRotPos, zoomDistance_);

	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_ = VAdd(followPos, cameraLocalRotPos);

	// カメラの上方向を計算
	VECTOR up = VTransform(AsoUtility::DIR_U, mat);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		up
	);
}

const VECTOR& Camera::GetTargetPos(void) const
{
	return targetPos_;
}


void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;
	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}
}

void Camera::SetFollow(Player* follow)
{
	follow_ = follow;
}

void Camera::MoveXYZDirection(void)
{
	auto& ins = InputManager::GetInstance();
	// 矢印キーでカメラの角度を変える
	float rotPow = 1.0f * DX_PI_F / 180.0f;

	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }

	// 上下の角度制限
	if (angles_.x > DX_PI_F / 6.0f) { angles_.x = DX_PI_F / 6.0f; }
	if (angles_.x < -DX_PI_F / 4.3f) { angles_.x = -DX_PI_F / 4.3f; }

}

void Camera::MoveXYZDirectionPad(void)
{
	auto& ins = InputManager::GetInstance();

	// 矢印キーでカメラの角度を変える
	float rotPow = 1.0f * DX_PI_F / 180.0f;

	// ゲームパッド操作
	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// アナログキーの入力値から方向を取得
	VECTOR dir = ins.GetDirectionXZAKey(padState.AKeyRX, padState.AKeyRY);

	// 右スティック上下の傾き
	angles_.x -= dir.z * rotPow * 2.0f;

	// 右スティック左右の傾き
	angles_.y += dir.x * rotPow * 2.0f;

	// 上下の角度制限
	if (angles_.x > DX_PI_F / 6.0f) { angles_.x = DX_PI_F / 6.0f; }
	if (angles_.x < -DX_PI_F / 6.0f) { angles_.x = -DX_PI_F / 6.0f; }
}

void Camera::SetZoomTarget(float zoom)
{
	zoomDistance_ = zoom;
	if (zoomDistance_ < 0.1f) zoomDistance_ = 0.1f; // 最小値制限
	if (zoomDistance_ > 5.0f) zoomDistance_ = 5.0f; // 最大値制限
}

void Camera::SetVictoryView(const VECTOR& pos, const VECTOR& target)
{
	// 固定カメラ用の設定
	ChangeMode(MODE::FIXED_POINT);
	pos_ = pos;
	targetPos_ = target;

	// 角度を自動で計算（注視点に向ける）
	VECTOR dir = VSub(target, pos);
	angles_.y = atan2f(dir.x, dir.z);
	angles_.x = -atan2f(dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));
}

void Camera::Release(void)
{
}

const VECTOR& Camera::GetPos(void) const
{
	return pos_;
}

const VECTOR& Camera::GetAngles(void) const
{
	return angles_;
}
