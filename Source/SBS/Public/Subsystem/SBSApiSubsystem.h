// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Structures/ApiJsonStruct.h"
#include "Structures/ApiPostStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SBSApiSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBlueprintQueryDone, const TArray<FBlueprintJsonStructure>&,
	Blueprints, int32, Max, bool, Success);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBlueprintPackQueryDone, const TArray<FBlueprintPackJsonStructure>&,
	BlueprintPacks, int32, Max, bool, Success);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDynamicQueryDone, FDynamicApiPostStruct, PostResult, bool, Success);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAuthUpdated, FSBSUserData, UserData, bool, Success);

/**
 * 
 */
UCLASS()
class SBS_API USBSApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	bool Tick(float DeltaSeconds);

	FTSTicker::FDelegateHandle TickDelegateHandle;

public:
	UFUNCTION(BlueprintCallable, Category = "KMods|Json")
	void QueryApi(FFilterPostStruct Post);

	UFUNCTION(BlueprintCallable, Category = "KMods|Json")
	void QueryPackApi(FPackFilterPostStruct Post);

	UFUNCTION(BlueprintCallable, Category = "KMods|Json")
	void QueryApiDynamic(FDynamicApiPostStruct Post);

	UFUNCTION(BlueprintCallable, Category = "KMods|Json")
	void QueryRating(FRatingPostStruct Post);

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	bool IsQuery() const;

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	bool IsPackQuery() const;

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	bool IsBpQuery() const;

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	bool IsAuthQuery() const;

	UFUNCTION(BlueprintCallable, Category = "KMods|Json")
	void QueryForAuth();

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE TArray<FBlueprintJsonStructure> GetBlueprints() const
	{
		return mCurrentBlueprints;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE TArray<FBlueprintPackJsonStructure> GetBlueprintPacks() const
	{
		return mCurrentBlueprintPacks;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE int32 GetTotalBlueprints() const
	{
		return mTotalBlueprints;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE int32 GetTotalBlueprintPacks() const
	{
		return mTotalBlueprintPacks;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE FSBSUserData GetUserData() const
	{
		return mUserData;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE bool IsLoggedIn() const
	{
		return mUserData.Role >= 0;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE bool GetIsInQuery() const
	{
		return bIsInQuery;
	}

	UFUNCTION(BlueprintPure, Category = "KMods|Json")
	FORCEINLINE TArray<FBlueprintJsonTagStructure> GetTags() const
	{
		return mTags;
	}

	// NATIVE GETTER
	static USBSApiSubsystem* Get(const UObject* WorldContext);

	UPROPERTY(BlueprintAssignable)
	FOnBlueprintQueryDone mOnQueryDone;

	UPROPERTY(BlueprintAssignable)
	FOnBlueprintPackQueryDone mOnPackQueryDone;

	UPROPERTY(BlueprintAssignable)
	FOnDynamicQueryDone mOnDynamicQueryDone;

	UPROPERTY(BlueprintAssignable)
	FOnAuthUpdated mOnAuthUpdated;

	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	TArray<FBlueprintJsonTagStructure> mTags;

protected:
	void OnBlueprintQueryDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	void OnBlueprintPackQueryDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	void OnQueryDynamicDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	void OnQueryAuthDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	UFUNCTION()
	void OnAccountKeyChanged();

private:
	UPROPERTY()
	TArray<FBlueprintJsonStructure> mCurrentBlueprints;

	UPROPERTY()
	TArray<FBlueprintPackJsonStructure> mCurrentBlueprintPacks;

	UPROPERTY()
	int32 mTotalBlueprints;

	UPROPERTY()
	int32 mTotalBlueprintPacks;

	UPROPERTY()
	FSBSUserData mUserData;

	UPROPERTY()
	FDynamicApiPostStruct mDynamicQuery;

	UPROPERTY()
	TArray<FDynamicApiPostStruct> mDynamicQueue;

	UPROPERTY()
	bool bIsInQuery = false;

	UPROPERTY()
	bool bIsInPackQuery = false;

	UPROPERTY()
	bool bIsInAuthQuery = false;
};