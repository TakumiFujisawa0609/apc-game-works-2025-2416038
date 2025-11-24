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
	image_ = LoadGraph("Data/Image/tutorial.png");
}

void Tutorial::Update(void)
{
	auto& ins = InputManager::GetInstance();

	// --- 決定（Aボタン or SPACE） ---
	bool decide =
		ins.IsTrgDown(KEY_INPUT_RETURN) ||  // キーボードSPACE
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::A); // Aボタン

	if (decide)
	{
		SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void Tutorial::Draw(void)
{
	DrawGraph(0, 0, image_, TRUE);
}

void Tutorial::Release(void)
{
}
