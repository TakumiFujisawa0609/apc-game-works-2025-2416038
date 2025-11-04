#include "../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
	: modelId_(modelId)
	, playType_(-1)
	, isLoop_(false)
	, isBlending_(false)
	, blendTime_(0.0f)
	, blendDuration_(0.25f) // デフォルトブレンド時間（秒） ? 好みで調整
	, prevAttachNo_(-1)
{
}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	Add(type, speed, animation);
}

void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;

	Add(type, speed, animation);
}

void AnimationController::Play(int type, bool isLoop)
{

	if (playType_ == type)
	{
		// 同じアニメーションだったら再生を継続する
		return;
	}


	// 現在再生中のものを prev に退避してブレンド開始
	if (playType_ != -1 && playAnim_.attachNo >= 0)
	{
		// もし既にブレンド中なら、前の prev を切り離す（リソース解放）
		if (isBlending_)
		{
			if (prevAttachNo_ >= 0)
			{
				MV1DetachAnim(modelId_, prevAttachNo_);
				prevAttachNo_ = -1;
			}
			prevAnim_ = playAnim_;
		}
		else
		{
			prevAnim_ = playAnim_;
		}

		prevAttachNo_ = prevAnim_.attachNo;
		isBlending_ = true;
		blendTime_ = 0.0f;
	}
	else
	{
		// no prev
		isBlending_ = false;
		prevAttachNo_ = -1;
	}

	// 新しい再生対象を設定
	playType_ = type;
	playAnim_ = animations_[type];
	playAnim_.step = 0.0f;
	isLoop_ = isLoop;

	// 新しいアニメをアタッチ（同一ファイル or 外部FBX）
	if (playAnim_.model == -1)
	{
		// 同一モデル内のアニメをアタッチ
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	}
	else
	{
		// 外部FBXからアタッチ（anim index 0 を使う場合が多い）
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	}

	// 総時間取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	// 初期ブレンドレート設定
	if (isBlending_ && prevAttachNo_ >= 0)
	{
		// 前アニメは1.0、新アニメは0.0から始める
		MV1SetAttachAnimBlendRate(modelId_, prevAttachNo_, 1.0f);
		MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, 0.0f);
	}
	else
	{
		MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, 1.0f);
	}

	//if (playType_ != -1)
	//{
	//	// モデルからアニメーションを外す
	//	MV1DetachAnim(modelId_, playAnim_.attachNo);
	//}

	//// アニメーション種別を変更
	//playType_ = type;
	//playAnim_ = animations_[type];

	//// 初期化
	//playAnim_.step = 0.0f;
	//isLoop_ = isLoop;　// アニメーションループ

	//// モデルにアニメーションを付ける
	//if (playAnim_.model == -1)
	//{
	//	// モデルと同じファイルからアニメーションをアタッチする
	//	playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	//}
	//else
	//{
	//	// 別のモデルファイルからアニメーションをアタッチする
	//	// DxModelViewerを確認すること(大体0か1)
	//	int animIdx = 0;
	//	playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	//}

	//// アニメーション総時間の取得
	//playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

}

void AnimationController::Update(void)
{
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance()->GetDeltaTime();

	if (playAnim_.attachNo >= 0)
	{
		playAnim_.step += deltaTime * playAnim_.speed;
		if (playAnim_.step > playAnim_.totalTime)
		{
			if (isLoop_)
				playAnim_.step = 0.0f;
			else
				playAnim_.step = playAnim_.totalTime;
		}

		MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
	}

	// advance prev animation time too (so blending looks correct)
	if (isBlending_ && prevAttachNo_ >= 0)
	{
		prevAnim_.step += deltaTime * prevAnim_.speed;
		if (prevAnim_.step > prevAnim_.totalTime)
		{
			if (isLoop_)
				prevAnim_.step = 0.0f;
			else
				prevAnim_.step = prevAnim_.totalTime;
		}
		MV1SetAttachAnimTime(modelId_, prevAttachNo_, prevAnim_.step);

		// update blend weight
		blendTime_ += deltaTime;
		float t = blendTime_ / blendDuration_;
		if (t > 1.0f) t = 1.0f;

		// prev: 1 -> 0, current: 0 -> 1
		MV1SetAttachAnimBlendRate(modelId_, prevAttachNo_, 1.0f - t);
		MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, t);

		// ブレンド終了判定
		if (t >= 1.0f)
		{
			// prev を detach
			if (prevAttachNo_ >= 0)
			{
				MV1DetachAnim(modelId_, prevAttachNo_);
				prevAttachNo_ = -1;
			}
			isBlending_ = false;
			blendTime_ = 0.0f;
		}
	}

//	// 再生
//	playAnim_.step += (deltaTime * playAnim_.speed);
//
//	// アニメーションが終了したら
//	if (playAnim_.step > playAnim_.totalTime)
//	{
//		if (isLoop_)
//		{
//			// ループ再生
//			playAnim_.step = 0.0f;
//		}
//		else
//		{
//			// ループしない
//			playAnim_.step = playAnim_.totalTime;
//		}
//	}
//
//	// アニメーション設定
//	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
//
//}
//
//void AnimationController::Release(void)
//{
//
//	// 外部FBXのモデル(アニメーション)解放
//	for (const std::pair<int, Animation>& pair : animations_)
//	{
//		if (pair.second.model != -1)
//		{
//			MV1DeleteModel(pair.second.model);
//		}
//	}
//
//	// 可変長配列をクリアする
//	animations_.clear();

}

void AnimationController::Release(void)
{
	// 外部FBXのモデル(アニメーション)解放 
	for (const std::pair<int, Animation>& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	} 

	// 再生中のアタッチははずす
	if (playAnim_.attachNo >= 0)
	{
		MV1DetachAnim(modelId_, playAnim_.attachNo);
		playAnim_.attachNo = -1;
	}
	if (prevAttachNo_ >= 0)
	{
		MV1DetachAnim(modelId_, prevAttachNo_);
		prevAttachNo_ = -1;
	}

	// 可変長配列をクリアする 
	animations_.clear();
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

void AnimationController::Add(int type, float speed, Animation& animation)
{
	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 追加
		animations_.emplace(type, animation);
	}
}
