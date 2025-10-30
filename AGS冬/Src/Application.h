#pragma once
#include <string>
class FpsController;

class Application
{

public:


	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_EFFECT;
	static const std::string PATH_MAP_DATA;

	//-------------------------------------------

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1920;
	static constexpr int SCREEN_SIZE_Y = 1080;
	static constexpr int COLOR_BIT_DIPTH = 32;

	// 固定FPS
	static constexpr int FRAME_RATE = 60;

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application* GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの破棄
	void Destroy(void);

	// エラー判定
	bool IsError(void);


private:

	// 静的インスタンス
	static Application* instance_;

	// コンストラクタ
	Application(void);

	// コピーコンストラクタ
	Application(const Application& ins);

	// デストラクタ
	~Application(void);

	// エラー判定
	bool isError_;

	// FPS
	FpsController* fpsController_;

};
