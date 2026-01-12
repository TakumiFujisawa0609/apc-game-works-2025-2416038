#include "SoundManager.h"

// ƒOƒ[ƒoƒ‹—Ìˆæ‚Å•Ï”‚ğ‰Šú‰»
SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager(void)
{
}

SoundManager::~SoundManager(void)
{
}

SoundManager::SoundManager(const SoundManager& sound)
{
}

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager;
	}
}

SoundManager* SoundManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager;
	}
	return instance_;
}

void SoundManager::DeleteInstance(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
	}
}

void SoundManager::Init(void)
{
	walkHundle_ = LoadSoundMem(WALK_PATH);
	fallingObjectHundle_ = LoadSoundMem(FALLING_OBJECT_PATH);
}

void SoundManager::Update(void)
{
}

void SoundManager::Release(void)
{
}

void SoundManager::PlayBgm1()
{
	//// Ä¶
	//if (CheckSoundMem(bgm1Hundle_) == 0)
	//{
	//	PlaySoundMem(bgm1Hundle_, DX_PLAYTYPE_LOOP, true);
	//}

	//// ‰¹—Ê’²®
	//ChangeVolumeSoundMem(BGM1_VOLUME, bgm1Hundle_);
}

void SoundManager::PlayWalk()
{
	// Ä¶
	if (CheckSoundMem(walkHundle_) == 0)
	{
		PlaySoundMem(walkHundle_, DX_PLAYTYPE_BACK, true);
	}

	// ‰¹—Ê’²®
	ChangeVolumeSoundMem(WALK_VOLUME, walkHundle_);
}

void SoundManager::PlayFallingObject()
{
	// Ä¶
	if (CheckSoundMem(fallingObjectHundle_) == 0)
	{
		PlaySoundMem(fallingObjectHundle_, DX_PLAYTYPE_BACK, true);
	}

	// ‰¹—Ê’²®
	ChangeVolumeSoundMem(FALLING_OBJECT_VOLUME, fallingObjectHundle_);
}

void SoundManager::StopBgm1()
{
	//StopSoundMem(bgm2Hundle_);
}

void SoundManager::StopWalk()
{
	StopSoundMem(walkHundle_);
}

void SoundManager::StopFallingObject()
{
	StopSoundMem(fallingObjectHundle_);
}
