#include "StageManager.h"

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	// ステージモデルの読み込み
	stageModelIds_.emplace_back(
		MV1LoadModel("Data/Model/Stage/panel.mv1"));

	// パネル1枚のサイズを取得
	startPos_ = VGet(-PANEL_SIZE, 0.0f, PANEL_SIZE); // 左上原点
	// ======== 3x3 グリッド配置 ========
	const int rowCount = 3;
	const int colCount = 3;

	// 決められた数配置
	for (int row = 0; row < rowCount; ++row)
	{
		for (int col = 0; col < colCount; ++col)
		{
			// 生成
			StageBase* panel = new StageBase();

			// 初期化
			panel->Init(
				stageModelIds_[static_cast<int>(StageBase::TYPE::PANEL)]);

			// 位置設定
			VECTOR pos = VGet(
				startPos_.x + col * PANEL_SIZE,
				startPos_.y,
				startPos_.z - row * PANEL_SIZE
			);

			panel->SetPos(pos);

			// ステージを登録
			stages_[StageBase::TYPE::PANEL].emplace_back(panel);
		}
	}
}

void StageManager::Update(void)
{
	// ステージの更新
	for (const auto pair : stages_)
	{
		for (StageBase* stage : pair.second)
		{
			stage->Update();
		}
	}
}

void StageManager::Draw(void)
{
	// ステージの描画
	for (const auto pair : stages_)
	{
		for (StageBase* stage : pair.second)
		{
			stage->Draw();
		}
	}
}

void StageManager::Release(void)
{
	// ステージの解放
	for (const auto pair : stages_)
	{
		for (StageBase* stage : pair.second)
		{
			stage->Release();
			delete stage;
		}
	}

	// ベースとなるモデルの開放
	for (int modelId : stageModelIds_)
	{
		MV1DeleteModel(modelId);
	}
}
