// Copyright Coffee Stain Studios. All Rights Reserved.


#include "BlueprintFunctionLib/SBSApiFunctionLibrary.h"

#include "FGBlueprintSubsystem.h"

bool USBSApiFunctionLibrary::IsBlueprintInstalled(UObject* WorldContext, FString Blueprint)
{
	if (IsValid(WorldContext))
	{
		AFGBlueprintSubsystem* Subsystem = AFGBlueprintSubsystem::Get(WorldContext->GetWorld());
		if (IsValid(Subsystem))
		{
			return Subsystem->DoesBlueprintExist(Blueprint);
		}
	}
	return false;
}
