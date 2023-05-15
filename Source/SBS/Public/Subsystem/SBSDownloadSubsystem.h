// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BFL/KBFL_Util.h"
#include "Interfaces/IHttpRequest.h"
#include "Structures/ApiJsonStruct.h"
#include "Subsystem/KPCLModSubsystem.h"
#include "SBSDownloadSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnDownloadComplete, FBlueprintJsonStructure, Blueprint, bool, Success );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FOnDownloadProgress, FBlueprintJsonStructure, Blueprint, float, Progress, FString, FileName );

UCLASS()
class SBS_API ASBSDownloadSubsystem : public AKPCLModSubsystem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBSDownloadSubsystem();

	virtual void BeginPlay() override;

	virtual void SubsytemTick( float dt ) override;

	UFUNCTION( BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Subsystem|SBS", DisplayName="GetSBSDownloadSubsystem" )
	static ASBSDownloadSubsystem* Get( UObject* WorldContext ) { return Cast< ASBSDownloadSubsystem >( UKBFL_Util::GetSubsystemFromChild( WorldContext, StaticClass() ) ); }

	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	bool DownloadBlueprint( FBlueprintJsonStructure Blueprint );

	// 0 = not pending; 1 = pending; 2 = failed
	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	int32 GetDownloadStateForBlueprint( FBlueprintJsonStructure Blueprint );

	UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
	FString GetCurrentBlueprintPath();

private:
	USBSApiSubsystem* mApiSubsystem;

	bool bDownloadFile1Completed;
	bool bDownloadFile2Completed;

	bool bDownloadFailed;

	void OnOneDownloadComplete();

	void OnDownloadCompleteFile1( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess );
	void OnDownloadCompleteFile2( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess );

	void OnDownloadProgressFile1( FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived ) const;
	void OnDownloadProgressFile2( FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived ) const;

	void CheckFilePath( FString filePath );

	UPROPERTY()
	FBlueprintJsonStructure mCurrentDownload;

	TArray< FBlueprintJsonStructure > mDownloadQueueChecker;
	TQueue< FBlueprintJsonStructure, EQueueMode::Spsc > mDownloadQueue;

	UPROPERTY( BlueprintAssignable )
	FOnDownloadComplete mFOnDownloadComplete;

	UPROPERTY( BlueprintAssignable )
	FOnDownloadProgress mOnDownloadProgress;
};
