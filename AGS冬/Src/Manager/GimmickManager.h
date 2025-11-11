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

    std::vector<GimmickBase*> gimmicks_;
    GimmickBase* currentGimmick_;

    int timer_;
};

