#pragma once
#include <vector>
#include <DxLib.h>
#include "../Gimmick/GimmickBase.h"

class GimmickFalling : public GimmickBase
{
public:
    struct PanelInfo {
        VECTOR pos;  // パネル中心座標
    };

    struct WaveInfo {
        VECTOR panelPos;     // パネル座標
        float warningTimer;  // 警告アニメ用タイマー
        bool isWarning;      // 警告中か
        bool isFalling;      // 落下中か
        float fallY;         // 現在の落下高さ
        int modelHandle;     // 個別落下物モデル
        int alpha;           // フェードアウト用
    };

    GimmickFalling();
    ~GimmickFalling() override;

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    GimmickType GetType() const override;

    std::vector<VECTOR> GetPositions() const override;

protected:

    void SetupWave();  // 1ウェーブの初期化

    std::vector<VECTOR> panels_;    // 9パネルの座標
    std::vector<WaveInfo> waveInfos_;  // 現在ウェーブの情報

    int modelId_;          // 落下物モデル
    int currentWave_;      // 現在ウェーブ番号
    float waveTimer_;      // ウェーブ内タイマー
    bool waveActive_;      // ウェーブ進行中か

    void InitLoad() override;
    void InitTransform() override;
    void InitPost() override;
};