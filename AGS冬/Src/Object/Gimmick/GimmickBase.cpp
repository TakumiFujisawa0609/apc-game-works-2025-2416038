#include "../../Manager/SceneManager.h"
#include "GimmickBase.h"

GimmickBase::GimmickBase(void)
{
	wave_ = WAVE::WAVE1;
	waveTimer_ = 0.0f;
}

GimmickBase::~GimmickBase(void)
{
}

void GimmickBase::Init(int baseModel)
{
	// ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	modelId_ = MV1DuplicateModel(baseModel);

	InitPost();

	isActive_ = true;
}

void GimmickBase::Update(void)
{
	switch (wave_)
	{
	case WAVE::WAVE1:
		UpdateWave1();
		break;
	case WAVE::WAVE2:
		UpdateWave2();
		break;
	case WAVE::WAVE3:
		UpdateWave3();
		break;
	}

	// 60•b—§‚Á‚½‚çWave‘JˆÚ
	waveTimer_ += SceneManager::GetInstance()->GetDeltaTime();

	if (wave_ == WAVE::WAVE1 && waveTimer_ >= 60.0f)
	{
		ChangeWave(WAVE::WAVE2);
	}
	else if (wave_ == WAVE::WAVE2 && waveTimer_ >= 120.0f)
	{
		ChangeWave(WAVE::WAVE3);
	}
}

void GimmickBase::Draw(void)
{
	DrawFormatString(500, 0, 0xffffff, "wave: %d", static_cast<int>(wave_));
}

void GimmickBase::Release(void)
{
	isActive_ = false;
}

bool GimmickBase::IsActive()
{
	return isActive_;
}

TYPE GimmickBase::GetType() const
{
	return TYPE::NONE;
}

VECTOR GimmickBase::GetPos() const
{
	return VGet(0.0f, -400.0f, 0.0f);
}

int GimmickBase::GetModelId() const
{
	return modelId_;
}

std::vector<VECTOR> GimmickBase::GetPositions() const
{
	return { VGet(0.0f, -400.0f, 0.0f) };
}

void GimmickBase::InitTransformPost(void)
{
	
}

void GimmickBase::ChangeWave(WAVE wave)
{
	wave_ = wave;

	switch (wave_)
	{
	case WAVE::WAVE1:
		break;
	case WAVE::WAVE2:
		break;
	case WAVE::WAVE3:
		break;
	}
}
