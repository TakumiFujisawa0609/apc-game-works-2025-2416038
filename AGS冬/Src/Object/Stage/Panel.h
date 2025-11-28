#pragma once
#include <DxLib.h>
#include "StageBase.h"

class Panel : public StageBase
{
public:

	Panel(void);
	~Panel(void);

	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	void InitColor(void) override;
};

