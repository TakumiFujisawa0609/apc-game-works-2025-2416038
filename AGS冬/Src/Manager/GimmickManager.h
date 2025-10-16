#pragma once
#include <vector>
#include <DxLib.h>
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
private:

    std::vector<GimmickBase*> gimmicks_;
    GimmickBase* currentGimmick_;

    int timer_;
};

