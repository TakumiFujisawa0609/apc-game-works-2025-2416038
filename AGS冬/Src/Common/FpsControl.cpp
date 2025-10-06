#include "DxLib.h"
#include "FpsControl.h"
#include "../Application.h"
#include "../Manager/InputManager.h"

FpsControl::FpsControl()
{
	currentTime = 0;
	prevFrameTime = 0;
	frameCnt = 0;
	updateFrameRateTime = 0;
	frameRate = 0.0f;
}

FpsControl::~FpsControl()
{
}

void FpsControl::Init(void)
{
	currentTime = 0;
	prevFrameTime = 0;
	frameCnt = 0;
	updateFrameRateTime = 0;
	frameRate = 0.0f;
}

// フレームレート更新
bool FpsControl::UpdateFrameRate(void)
{
	Sleep(1);	// システムに処理を返す

	// 現在の時刻を表示
	currentTime = GetNowCount();

	// 現在の時刻が、前回のフレーム実行時より
	// 1/60秒経過していたら処理を実行する
	if (currentTime - prevFrameTime >= FRAME_RATE)
	{
		// フレームレート実行時の時間を更新
		prevFrameTime = currentTime;

		// フレーム数をカウント
		frameCnt++;

		// 1/60秒経過した
		return true;
	}

	return false;
}

void FpsControl::CalcFrameRate(void)
{
	// 前回のフレームレート更新から経過時間を求める
	int difTime = currentTime - updateFrameRateTime;

	// 前回のフレームレート更新から
	// １秒以上経過していたらフレームレートを更新する
	if (difTime > 1000)
	{
		// フレーム回数をミリ秒に合わせる
		// 小数まで出したのでfloatにキャスト
		float castFrameCnt = (float)(frameCnt * 1000);

		// フレームレートを求める
		// 理想通りなら 60000 / 1000 で　60　となる
		frameRate = castFrameCnt / difTime;

		// フレームカウントをクリア
		frameCnt = 0;

		// フレームレート更新時間を更新
		updateFrameRateTime = currentTime;
	}
}

// FPS表示
void FpsControl::DrawFrameRate(void)
{

	// フォントサイズ変更
	SetFontSize(18);

	//// スクリーンの右端に来るように設定
	//DrawFormatString(
	//	Application::SCREEN_SIZE_X - 90,
	//	0,
	//	0xffffff,
	//	"FPS[%.2f]",
	//	frameRate
	//);
	// フォントサイズ固定
	SetFontSize(18);
}
