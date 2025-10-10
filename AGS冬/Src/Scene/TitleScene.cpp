#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
    cursorIndex_ = 0;
    moveDelay_ = 0;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
}

void TitleScene::Update(void)
{
	auto& ins = InputManager::GetInstance();

	// メニュー更新
	UpdateMenu();
}

void TitleScene::Draw(void)
{
	DrawFormatString(
		300, 200, 0xffffff,
		"Title Scene"
	);

    const char* menu[] = { "GAME START", "OPTION", "EXIT" };
    for (int i = 0; i < 3; i++)
    {
        int color = (i == cursorIndex_) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        DrawFormatString(800, 200 + i * 40, color, menu[i]);
    }
}

void TitleScene::Release(void)
{
}

void TitleScene::UpdateMenu(void)
{
    auto& ins = InputManager::GetInstance();

    VECTOR dir = ins.GetMenuInputDir();

    // 入力間隔（押しっぱなし防止）
    if (moveDelay_ > 0) moveDelay_--;

    // --- カーソル移動（十字キー／スティック） ---
    if (moveDelay_ == 0)
    {
        // ↑キー
        if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP) ||
            dir.z < 0)
        {
            cursorIndex_--;
            moveDelay_ = 15;
        }

        // ↓キー
        if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) || dir.z > 0)
        {
            cursorIndex_++;
            moveDelay_ = 15;
        }

        // カーソル範囲制御
        if (cursorIndex_ < 0) cursorIndex_ = 2;
        if (cursorIndex_ > 2) cursorIndex_ = 0;
    }

    // --- 決定（Aボタン or SPACE） ---
    bool decide =
        ins.IsTrgDown(KEY_INPUT_SPACE) ||  // キーボードSPACE
        ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::A); // Aボタン

    if (decide)
    {
        switch (cursorIndex_)
        {
        case 0: // GAME START
            SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::GAME);
            break;

        case 1: // OPTION
            // SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::OPTION);
            break;

        case 2: // EXIT
            DxLib_End();
            break;
        }
    }
}
