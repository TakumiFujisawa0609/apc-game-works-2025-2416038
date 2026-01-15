#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
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
    // カメラ
    camera_ = SceneManager::GetInstance()->GetCamera();
    camera_->ChangeMode(Camera::MODE::FIXED_POINT);

    // テレビモデル読み込み
	tvModelId_ = MV1LoadModel("Data/Model/Stage/TV.mv1");
    // 床
	floorModelId_ = MV1LoadModel("Data/Model/Stage/floor.mv1");

    MV1SetPosition(tvModelId_, VGet(670.0f, 0.0f, 600.0f));
	MV1SetPosition(floorModelId_, VGet(670.0f, 0.0f, 600.0f));
  
    //マテリアルをすべてエミッシブに設定
    int materialNum = MV1GetMaterialNum(floorModelId_);
    for (int i = 0; i < materialNum; ++i)
    {
        MV1SetMaterialDifColor(floorModelId_, i, GetColorF(0.5f, 0.5f, 0.5f, 1)); // 拡散反射
        MV1SetMaterialSpcColor(floorModelId_, i, GetColorF(0.5f, 0.5f, 0.5f, 1)); // 鏡面反射
        MV1SetMaterialEmiColor(floorModelId_, i, GetColorF(0.4f, 0.4f, 0.4f, 1.0f)); // 自発光
        MV1SetMaterialAmbColor(floorModelId_, i, GetColorF(1, 1, 1, 1)); // 環境光
    }

    // フォントロード
    funwariFontHandle_ = CreateFontToHandle("Showcard Gothic", 80, 2, DX_FONTTYPE_ANTIALIASING);
}

void TitleScene::Update(void)
{
    SetBackgroundColor(0, 0, 0);

	// メニュー更新
	UpdateMenu();
    
    //if (CheckHitKey(KEY_INPUT_W)) { pointLightPos_.z += 30; }
    //if (CheckHitKey(KEY_INPUT_A)) { pointLightPos_.x -= 30; }
    //if (CheckHitKey(KEY_INPUT_S)) { pointLightPos_.z -= 30; }
    //if (CheckHitKey(KEY_INPUT_D)) { pointLightPos_.x += 30; }
    //if (CheckHitKey(KEY_INPUT_UP)) { pointLightPos_.y += 30; }
    //if (CheckHitKey(KEY_INPUT_DOWN)) { pointLightPos_.y -= 30; }
}

void TitleScene::Draw(void)
{
    MV1DrawModel(tvModelId_);
	MV1DrawModel(floorModelId_);

    const char* menu[] = { "GAME START", "TUTORIAL", "EXIT" };
    for (int i = 0; i < 3; i++)
    {
        int color = (i == cursorIndex_) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
;
        DrawFormatStringToHandle(
            1270, 270 + i * 200,
            color,
            funwariFontHandle_,
            menu[i]
        );
    }

    //ライト設定
    ChangeLightTypePoint(
        VGet(570, 490,-430),
        1000.0f, 0.000f, 0.001f, 0.000f
    );

 //   // ライト座標
 //   DrawFormatString(10, 10, GetColor(255, 255, 255),
 //       "Light Pos X: %.1f Y: %.1f Z: %.1f",
 //       pointLightPos_.x, pointLightPos_.y, pointLightPos_.z
	//);
}

void TitleScene::Release(void)
{
	MV1DeleteModel(tvModelId_);
	MV1DeleteModel(floorModelId_);
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
        ins.IsTrgDown(KEY_INPUT_RETURN) ||  // キーボードSPACE
        ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::A); // Aボタン

    if (decide)
    {
        switch (cursorIndex_)
        {
        case 0: // GAME START
            SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
            break;

        case 1: // Tutprial
            SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
            break;

        case 2: // EXIT
            DxLib_End();
            break;
        }
    }
}
