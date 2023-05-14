// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structures/ApiPostStruct.h"
#include "SBSApiSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBlueprintQueryDone, const TArray<FBlueprintJsonStructure>&, Blueprints, int32, Max, bool, Success);

/**
 * 
 */
UCLASS()
class SBS_API USBSApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	void QueryApi( FFilterPostStruct Post );
	
	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	FORCEINLINE TArray<FBlueprintJsonStructure> GetBlueprints() const { return mCurrentBlueprints; }
	
	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	FORCEINLINE int32 GetTotalBlueprints() const { return mTotalBlueprints; }

	// NATIVE GETTER
	static USBSApiSubsystem* Get( const UObject* WorldContext );

	UPROPERTY(BlueprintAssignable);
	FOnBlueprintQueryDone mOnQueryDone;
	
private:
	UPROPERTY();
	TArray<FBlueprintJsonStructure> mCurrentBlueprints;
	
	UPROPERTY();
	int32 mTotalBlueprints;
};
