// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SBSApiFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SBS_API USBSApiFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Subsystem|SBS",
		DisplayName="GetSBSDownloadSubsystem")
	static bool IsBlueprintInstalled(UObject* WorldContext, FString Blueprint);
};