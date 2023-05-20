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

UCLASS( )
class SBS_API ASBSDownloadSubsystem : public AKPCLModSubsystem {
	GENERATED_BODY( )

	public:
		// Sets default values for this actor's properties
		ASBSDownloadSubsystem( );

		virtual void BeginPlay( ) override;

		virtual void SubsytemTick( float dt ) override;

		UFUNCTION( BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Subsystem|SBS", DisplayName="GetSBSDownloadSubsystem" )
		static ASBSDownloadSubsystem* Get( UObject* WorldContext ) {
			return Cast< ASBSDownloadSubsystem >( UKBFL_Util::GetSubsystemFromChild( WorldContext, StaticClass( ) ) );
		}

		UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
		bool DownloadBlueprint( FBlueprintJsonStructure Blueprint );

		UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
		bool DownloadBlueprintPack( FBlueprintPackJsonStructure BlueprintPack );

		// 0 = not pending; 1 = pending; 2 = running
		UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
		int32 GetDownloadStateForBlueprint( FBlueprintJsonStructure Blueprint );

		// 0 = not pending; 1 = running
		UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
		bool GetDownloadStateForBlueprintPack( FBlueprintPackJsonStructure BlueprintPack );

		UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
		FString GetCurrentBlueprintPath( );

		UFUNCTION( BlueprintCallable, Category = "KMods|Json" )
		bool IsBlueprintInstalled( FBlueprintJsonStructure Blueprint );

		UFUNCTION( BlueprintNativeEvent, Category = "KMods|Json" )
		bool OnBlueprintCreated( UFGBlueprintDescriptor* BlueprintDescriptor, FBlueprintJsonStructure Blueprint );

		UFUNCTION( BlueprintPure, Category = "KMods|Json" )
		static UFGLocalPlayer* SBS_GetLocalPlayer( UObject* WorldContext ) {
			return Cast< UFGLocalPlayer >( UKBFL_Player::GetFgPlayerState( WorldContext )->GetOwningController( )->GetLocalPlayer( ) );
		};

		UPROPERTY( BlueprintReadOnly )
		FString mSaveSessionName = FString( );

	private:
		UPROPERTY( )
		USBSApiSubsystem* mApiSubsystem;

		bool bDownloadFile1Completed;
		bool bDownloadFile2Completed;

		bool bDownloadFailed;

		void OnOneDownloadComplete( );

		void OnDownloadCompleteFile1( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess );

		void OnDownloadCompleteFile2( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess );

		void OnDownloadProgressFile1( FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived ) const;

		void OnDownloadProgressFile2( FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived ) const;

		void CheckFilePath( FString filePath );

		UPROPERTY( )
		FBlueprintJsonStructure mCurrentDownload;

		TArray< FBlueprintJsonStructure > mDownloadQueueChecker;

		UPROPERTY( BlueprintAssignable )
		FOnDownloadComplete mFOnDownloadComplete;

		UPROPERTY( BlueprintAssignable )
		FOnDownloadProgress mOnDownloadProgress;

		UPROPERTY( )
		AFGBlueprintSubsystem* mBlueprintSubsystem;
};
