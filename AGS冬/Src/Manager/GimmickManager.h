#pragma once
#include <map>
#include <vector>
#include <DxLib.h>
#include "../Object/Gimmick/GimmickBase.h"
class GimmickBase;

class GimmickManager
{
    struct PanelArea {
        VECTOR center;   // パネルの中心座標
        bool isWarning;  // 警告中フラグ
        float timer;     // 警告時間
    };

    enum class WAVE
    {
        WAVE1,
        WAVE2,
        WAVE3,
    };


public:

    GimmickManager();
    ~GimmickManager();

    void Init();
    void Update();
    void Draw();
    void Release();

    int GetModelId() const;

    VECTOR GetLaserPos();
    std::vector<VECTOR> GetFallingPos();

    GimmickBase& GetGimmicks();

    std::vector<int> GetLaserModelIds() const;
    std::vector<int> GetFallingObjectModelIds() const;

private:

    WAVE wave_;

    std::vector<GimmickBase*> gimmicks_;
    GimmickBase* currentGimmick_;

    // ギミックモデルのハンドルID
	std::vector<int> gimmickModelIds_;

    // ウェーブタイマー
    float waveTimer_;

    // 画像
    int img100kg_;
    int imgLazer_;

    int timer_;

    float firstDelayTimer_;
    float firstDelayDuration_;

    // 難易度（ウェーブ）変更
    void ChangeWave(WAVE wave);
};

