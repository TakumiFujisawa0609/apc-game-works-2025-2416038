#pragma once
#include <DxLib.h>
#include "StageBase.h"

class Monitor : public StageBase
{
public:

	Monitor(void);
	~Monitor(void);


private:


	void InitColor(void) override;
};

