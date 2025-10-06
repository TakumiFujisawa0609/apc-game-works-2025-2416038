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

	// 決められた数配置
	for (int i = 0; i < PANEL_NUM; i++)
	{
		// 生成
		StageBase* panel = new StageBase();
		
		// 初期化
		panel->Init(stageModelIds_[static_cast<int>(StageBase::TYPE::PANEL)]);

		// ステージを登録
		stages_[StageBase::TYPE::PANEL].emplace_back(panel);
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
