#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "TutorialScene.h"

Tutorial::Tutorial(void)
{
}

Tutorial::~Tutorial(void)
{
}

void Tutorial::Init(void)
{
	tutorialImg_[0] = LoadGraph("Data/Image/Tutorial1.png");
	tutorialImg_[1] = LoadGraph("Data/Image/Tutorial2.png");
	tutorialImg_[2] = LoadGraph("Data/Image/Tutorial3.png");

	pageIndex_ = 0; // 最初は1枚目
}

void Tutorial::Update(void)
{
	auto& ins = InputManager::GetInstance();

	// --- 決定（Aボタン or SPACE） ---
	bool decide =
		ins.IsTrgDown(KEY_INPUT_RETURN) ||  // ENTERキー
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::A); // Aボタン
	// 選択
	bool left =
		ins.IsTrgDown(KEY_INPUT_LEFT) ||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT); // 十字ボタン
	bool right =
		ins.IsTrgDown(KEY_INPUT_RIGHT) ||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT); // 十字ボタン

	// 十字キーでページめくり
	if (right && pageIndex_ <= 1)
	{
		pageIndex_++;
	}
	if (left && pageIndex_ >= 1)
	{
		pageIndex_--;
	}
	// 3枚見終わったらTITLEへ
	if (pageIndex_ == 2 && decide)
	{
		SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void Tutorial::Draw(void)
{
	DrawGraph(0, 0, tutorialImg_[pageIndex_], TRUE);
}


void Tutorial::Release(void)
{
	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(tutorialImg_[i]);
	}
}
