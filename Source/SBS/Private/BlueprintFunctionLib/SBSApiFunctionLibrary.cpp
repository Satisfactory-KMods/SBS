// Copyright Coffee Stain Studios. All Rights Reserved.


#include "BlueprintFunctionLib/SBSApiFunctionLibrary.h"

#include "Subsystems/KBFLAssetDataSubsystem.h"

bool USBSApiFunctionLibrary::IsApiSubsystemReady( UObject* WorldContextObject )
{
	if( IsValid( WorldContextObject ) )
	{
		return WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem< UKBFLAssetDataSubsystem >();
	}
	return nullptr;
}
