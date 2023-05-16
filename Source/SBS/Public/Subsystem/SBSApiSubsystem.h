// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Structures/ApiJsonStruct.h"
#include "Structures/ApiPostStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SBSApiSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FOnBlueprintQueryDone, const TArray<FBlueprintJsonStructure>&, Blueprints, int32, Max, bool, Success );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnDynamicQueryDone, FDynamicApiPostStruct, PostResult, bool, Success );

/**
 * 
 */
UCLASS()
class SBS_API USBSApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

public:
	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	void QueryApi( FFilterPostStruct Post );

	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	void QueryApiDynamic( FDynamicApiPostStruct Post );

	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	void QueryRating( FRatingPostStruct Post );

	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	bool IsQuery() const;

	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	bool HasAuth() const;

	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	FORCEINLINE TArray< FBlueprintJsonStructure > GetBlueprints() const { return mCurrentBlueprints; }

	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	FORCEINLINE int32 GetTotalBlueprints() const { return mTotalBlueprints; }

	UFUNCTION( BlueprintPure, Category = "KMods|Json" )
	FORCEINLINE bool GetIsInQuery() const { return bIsInQuery; }

	// NATIVE GETTER
	static USBSApiSubsystem* Get( const UObject* WorldContext );

	UPROPERTY( BlueprintAssignable )
	FOnBlueprintQueryDone mOnQueryDone;

	UPROPERTY( BlueprintAssignable )
	FOnDynamicQueryDone mOnDynamicQueryDone;

protected:
	void OnBlueprintQueryDone( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess );
	void OnQueryDynamicDone( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess );
	void OnAccountKeyChanged();
	void OnCheckKey( FDynamicApiPostStruct PostResult, bool Success );

private:
	UPROPERTY()
	TArray< FBlueprintJsonStructure > mCurrentBlueprints;

	UPROPERTY()
	FDynamicApiPostStruct mDynamicQuery;

	UPROPERTY()
	int32 mTotalBlueprints;

	UPROPERTY()
	bool bIsInQuery = false;

	UPROPERTY()
	bool bHasAuth = false;
};
