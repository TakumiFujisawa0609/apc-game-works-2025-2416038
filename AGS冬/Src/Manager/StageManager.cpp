#include "StageManager.h"
#include "../Object/Stage/Panel.h"
#include "../Object/Stage/Monitor.h"

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
		MV1LoadModel("Data/Model/Stage/asiba.mv1"));

	// モニターモデルの読み込み
	stageModelIds_.emplace_back(
		MV1LoadModel("Data/Model/monita.mv1"));

	for (int z = 0; z < DIVISIONS; z++)
	{
		for (int x = 0; x < DIVISIONS; x++)
		{
			// 生成
			StageBase* panel = new Panel();

			// 初期化
			panel->Init(
				stageModelIds_[static_cast<int>(StageBase::TYPE::PANEL)]);

			// 配置座標
			VECTOR pos = VGet(
				STARTX + x * STEP,
				0.0f,
				STARTZ + z * STEP
			);

			// 位置設定
			panel->SetPos(pos);

			// ステージを登録
			stages_[StageBase::TYPE::PANEL].emplace_back(panel);
		}
	}

	// モニターの生成
	StageBase* monitor = new Monitor();

	// 初期化
	monitor->Init(
		stageModelIds_[static_cast<int>(StageBase::TYPE::MONITOR)]);

	// 位置・大きさ・回転設定
	monitor->SetPos(POS_MONITOR);
	monitor->SetScales(SCL_MONITOR);
	monitor->SetRot(ROT_MONITOR);

	// ステージを登録
	stages_[StageBase::TYPE::MONITOR].emplace_back(monitor);

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

const std::map<StageBase::TYPE, std::vector<StageBase*>>& StageManager::GetStages()
{
	return stages_;
}
