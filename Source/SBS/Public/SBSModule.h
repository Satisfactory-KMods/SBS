#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FSBSModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override
	{
		return true;
	}
};
