#pragma once
#include <DxLib.h>

#define WALK_PATH "Data/Sound/足音.wav"
#define FALLING_OBJECT_PATH "Data/Sound/落下物.wav"
#define DAMAGE_PATH "Data/Sound/足音.wav"

class SoundManager
{
public:

	static constexpr int BGM_VOLUME = 153;		// BGMの音量（０～２５５）
	static constexpr int WALK_VOLUME = 200;		// 歩行音の音量（０～２５５）
	static constexpr int FALLING_OBJECT_VOLUME = 200;		// 歩行音の音量（０～２５５）

	// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static SoundManager* GetInstance(void);
	// インスタンスの削除
	static void DeleteInstance(void);

	void Init(void);
	void Update(void);
	void Release(void);

	// 音源再生
	void PlayBgm1();	// BGM
	void PlayWalk();	// 歩行音
	void PlayFallingObject(); // 落下物音

	//音源停止
	void StopBgm1();	// BGM
	void StopWalk();	// 歩行音
	void StopFallingObject(); // 落下物音

private:

	//コンストラクタ・デストラクタ（外部からの生成防止）
	SoundManager(void);
	~SoundManager(void);
	//コピーコンストラクタ
	SoundManager(const SoundManager& sound);

	// インスタンスを持たせる
	static SoundManager* instance_;

	// 音源ハンドル
	int walkHundle_;
	int fallingObjectHundle_;

};