// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SBSDownloadSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "SBS/Public/Logging.h"
#include "Subsystem/SBSApiSubsystem.h"
#include "FGNetworkLibrary.h"
#include "FGOnlineSessionSettings.h"
#include "FindSessionsCallbackProxy.h"

ASBSDownloadSubsystem::ASBSDownloadSubsystem()
{
	bUseSubsystemTick = false;
}

void ASBSDownloadSubsystem::BeginPlay()
{
	Super::BeginPlay();
	mApiSubsystem = USBSApiSubsystem::Get( GetWorld() );
	UE_LOG( LogSBS, Log, TEXT( "%s" ), *GetCurrentBlueprintPath() );
}

void ASBSDownloadSubsystem::SubsytemTick( float dt )
{
	Super::SubsytemTick( dt );

	if( IsValid( mApiSubsystem ) )
	{
		if( !mDownloadQueue.IsEmpty() && mCurrentDownload.ID.IsEmpty() )
		{
			bDownloadFailed = false;
			bDownloadFile1Completed = false;
			bDownloadFile2Completed = false;
			if( mDownloadQueue.Enqueue( mCurrentDownload ) )
			{
				mDownloadQueueChecker.Remove( mCurrentDownload );
				FDownloadSbpStruct SbpStruct;
				FDownloadSbpcfgStruct SbpcfgStruct;
				SbpStruct.ID = mCurrentDownload.ID;
				SbpcfgStruct.ID = mCurrentDownload.ID;

				const FHttpRequestPtr Download1 = FHttpModule::Get().CreateRequest();
				const FHttpRequestPtr Download2 = FHttpModule::Get().CreateRequest();

				Download1->SetURL( SbpStruct.getUrl() );
				Download2->SetURL( SbpcfgStruct.getUrl() );

				TMap< FString, FString > Headers;
				FApiPostStruct::MakeHeader( Headers );
				for( auto Header : Headers )
				{
					if( !Header.Key.IsEmpty() && !Header.Value.IsEmpty() )
					{
						Download1->SetHeader( Header.Key, Header.Value );
						Download2->SetHeader( Header.Key, Header.Value );
					}
				}

				Download1->SetVerb( "GET" );
				Download2->SetVerb( "GET" );

				Download1->OnProcessRequestComplete().BindUObject( this, &ASBSDownloadSubsystem::OnDownloadCompleteFile1 );
				Download1->OnRequestProgress().BindUObject( this, &ASBSDownloadSubsystem::OnDownloadProgressFile1 );

				Download2->OnProcessRequestComplete().BindUObject( this, &ASBSDownloadSubsystem::OnDownloadCompleteFile2 );
				Download2->OnRequestProgress().BindUObject( this, &ASBSDownloadSubsystem::OnDownloadProgressFile2 );

				Download1->ProcessRequest();
				Download2->ProcessRequest();
			}
		}
	}
}

void ASBSDownloadSubsystem::OnOneDownloadComplete()
{
	if( bDownloadFile1Completed && bDownloadFile2Completed )
	{
		if( !bDownloadFailed )
		{
			// Download succeeded
		}
		if( mFOnDownloadComplete.IsBound() )
		{
			mFOnDownloadComplete.Broadcast( mCurrentDownload, !bDownloadFailed );
		}
		mCurrentDownload.ID.Empty();
	}
}

void ASBSDownloadSubsystem::OnDownloadCompleteFile1( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess )
{
	if( !( bSuccess && Response.IsValid() && Response->GetResponseCode() == 200 ) )
	{
		bDownloadFailed = true;
	}
	else
	{
	}
	bDownloadFile1Completed = true;
	OnOneDownloadComplete();
}

void ASBSDownloadSubsystem::OnDownloadCompleteFile2( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess )
{
	if( !( bSuccess && Response.IsValid() && Response->GetResponseCode() == 200 ) )
	{
		bDownloadFailed = true;
	}
	else
	{
	}
	bDownloadFile1Completed = true;
	OnOneDownloadComplete();
}

void ASBSDownloadSubsystem::OnDownloadProgressFile1( FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived ) const
{
	int32 ReceivedSize = InBytesReceived;
	FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

	if( HttpResponse.IsValid() )
	{
		int32 TotalSize = HttpResponse->GetContentLength();
		float PercentDone = ( static_cast< float >( ReceivedSize ) / static_cast< float >( TotalSize ) ) * 100;
		if( mOnDownloadProgress.IsBound() )
		{
			mOnDownloadProgress.Broadcast( mCurrentDownload, PercentDone, TEXT( ".sbp" ) );
		}
	}
}

void ASBSDownloadSubsystem::OnDownloadProgressFile2( FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived ) const
{
	int32 ReceivedSize = InBytesReceived;

	FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

	if( HttpResponse.IsValid() )
	{
		int32 TotalSize = HttpResponse->GetContentLength();
		float PercentDone = ( static_cast< float >( ReceivedSize ) / static_cast< float >( TotalSize ) ) * 100;
		if( mOnDownloadProgress.IsBound() )
		{
			mOnDownloadProgress.Broadcast( mCurrentDownload, PercentDone, TEXT( ".sbpcfg" ) );
		}
	}
}

void ASBSDownloadSubsystem::CheckFilePath( FString filePath )
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
}


bool ASBSDownloadSubsystem::DownloadBlueprint( FBlueprintJsonStructure Blueprint )
{
	if( !mDownloadQueueChecker.Contains( Blueprint ) && mCurrentDownload != Blueprint )
	{
		mDownloadQueue.Enqueue( Blueprint );
		mDownloadQueueChecker.Add( Blueprint );
		return true;
	}
	return false;
}

int32 ASBSDownloadSubsystem::GetDownloadStateForBlueprint( FBlueprintJsonStructure Blueprint )
{
	if( Blueprint == mCurrentDownload )
	{
		return 2;
	}
	if( mDownloadQueueChecker.Contains( Blueprint ) )
	{
		return 1;
	}
	return 0;
}

FString ASBSDownloadSubsystem::GetCurrentBlueprintPath()
{
	AFGPlayerState* PlayerState = UKBFL_Player::GetFgPlayerState( GetWorld() );
	if( PlayerState )
	{
		UFGLocalPlayer* LocalPlayer = Cast< UFGLocalPlayer >( PlayerState->GetOwningController()->GetLocalPlayer() );
		if( LocalPlayer )
		{
			const FBlueprintSessionResult SessionResult = UFGSessionLibrary::GetMySession( LocalPlayer );
			const FFGOnlineSessionSettings SessionSettings = UFGSessionLibrary::GetSessionSettings( SessionResult );
			return UFGSaveSystem::GetSaveDirectoryPath() + "blueprints/" + SessionSettings.SaveSessionName;
		}
	}
	return FString();
}
