// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"

#include "SBSDefaultRCO.generated.h"

/**
 * 
 */
UCLASS()
class SBS_API USBSDefaultRCO : public UFGRemoteCallObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty >& OutLifetimeProps ) const override;

	UPROPERTY( Replicated )
	bool mDummy2 = true;
};
