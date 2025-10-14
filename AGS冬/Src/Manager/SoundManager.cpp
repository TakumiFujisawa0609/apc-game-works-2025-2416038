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
	//voiceHundle_ = LoadSoundMem(VOICE_PATH);
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

void SoundManager::StopBgm1()
{
	//StopSoundMem(bgm2Hundle_);
}
