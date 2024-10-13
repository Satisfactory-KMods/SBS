// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SBSDownloadSubsystem.h"

#include "FGBlueprintSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "SBS/Public/Logging.h"
#include "Subsystem/SBSApiSubsystem.h"

ASBSDownloadSubsystem::ASBSDownloadSubsystem()
{
	bUseSubsystemTick = false;
}

void ASBSDownloadSubsystem::BeginPlay()
{
	Super::BeginPlay();
	mApiSubsystem = USBSApiSubsystem::Get(GetWorld());
	mCurrentDownload.ID.Empty();
	mBlueprintSubsystem = AFGBlueprintSubsystem::Get(GetWorld());
}

void ASBSDownloadSubsystem::SubsytemTick(float dt)
{
	if (IsValid(mApiSubsystem))
	{
		if (mDownloadQueueChecker.Num() > 0 && mCurrentDownload.ID.IsEmpty())
		{
			bDownloadFailed = false;
			bDownloadFile1Completed = false;
			bDownloadFile2Completed = false;

			mCurrentDownload = mDownloadQueueChecker.Pop();
			UE_LOG(LogSBS, Log, TEXT("Download now '%s' with SBS Id: '%s'"), *mCurrentDownload.OriginalName,
				*mCurrentDownload.ID);
			FDownloadSbpStruct    SbpStruct;
			FDownloadSbpcfgStruct SbpcfgStruct;
			SbpStruct.ID = mCurrentDownload.ID;
			SbpcfgStruct.ID = mCurrentDownload.ID;

			const FHttpRequestPtr Download1 = FHttpModule::Get().CreateRequest();
			const FHttpRequestPtr Download2 = FHttpModule::Get().CreateRequest();

			Download1->SetURL(SbpStruct.getUrl(GetWorld()));
			Download2->SetURL(SbpcfgStruct.getUrl(GetWorld()));

			TMap<FString, FString> Headers;
			FApiPostStruct::MakeHeader(Headers, GetWorld());
			for (auto Header : Headers)
			{
				if (!Header.Key.IsEmpty() && !Header.Value.IsEmpty())
				{
					Download1->SetHeader(Header.Key, Header.Value);
					Download2->SetHeader(Header.Key, Header.Value);
				}
			}

			Download1->SetVerb("GET");
			Download2->SetVerb("GET");

			Download1->OnProcessRequestComplete().BindUObject(this, &ASBSDownloadSubsystem::OnDownloadCompleteFile1);
			Download1->OnRequestProgress().BindUObject(this, &ASBSDownloadSubsystem::OnDownloadProgressFile1);

			Download2->OnProcessRequestComplete().BindUObject(this, &ASBSDownloadSubsystem::OnDownloadCompleteFile2);
			Download2->OnRequestProgress().BindUObject(this, &ASBSDownloadSubsystem::OnDownloadProgressFile2);

			Download1->ProcessRequest();
			Download2->ProcessRequest();
		}
	}
}

void ASBSDownloadSubsystem::OnOneDownloadComplete()
{
	if (bDownloadFile1Completed && bDownloadFile2Completed)
	{
		if (!bDownloadFailed)
		{
			UE_LOG(LogSBS, Warning, TEXT("OnOneDownloadComplete '%s'"), *mCurrentDownload.ID);
			UFGBlueprintDescriptor* BP = mBlueprintSubsystem->GetBlueprintDescriptorByName(
				FText::FromString(mCurrentDownload.OriginalName));
			if (BP)
			{
				//mBlueprintSubsystem->Delet( BP );
			}

			mBlueprintSubsystem->ReadBlueprintFromDisc(mCurrentDownload.OriginalName);
			TArray<FBlueprintHeader> outHeaders;
			mBlueprintSubsystem->FindBlueprintHeaders(GetCurrentBlueprintPath(), outHeaders);
			//mBlueprintSubsystem->Init();
			mBlueprintSubsystem->GenerateManifest();
			mBlueprintSubsystem->EnumerateBlueprints();
			mBlueprintSubsystem->EnumerateBlueprintConfigs();
			//mBlueprintSubsystem->CreateBlueprintCategoriesFromRecords( );
			mBlueprintSubsystem->MarkRecordDataDirty();
			mBlueprintSubsystem->RefreshBlueprintsAndDescriptors();
		}

		if (mFOnDownloadComplete.IsBound())
		{
			mFOnDownloadComplete.Broadcast(mCurrentDownload, !bDownloadFailed);
		}
		mCurrentDownload.ID.Empty();
	}
}

void ASBSDownloadSubsystem::OnDownloadCompleteFile1(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!(bSuccess && Response.IsValid() && Response->GetResponseCode() == 200))
	{
		bDownloadFailed = true;
	}
	else
	{
		FString Path = GetCurrentBlueprintPath() + "/" + mCurrentDownload.OriginalName + ".sbp";
		CheckFilePath(Path);
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		bool         Failed = false;
		IFileHandle* FileHandle = PlatformFile.OpenWrite(*Path);
		if (!FileHandle)
		{
			UE_LOG(LogSBS, Error, TEXT("Something went wrong while saving the file '%s'"), *Path);
			Failed = true;
		}

		if (!FileHandle->Write(Response->GetContent().GetData(), Response->GetContentLength()))
		{
			UE_LOG(LogSBS, Error, TEXT("Something went wrong while writing the response data to the file '%s'"), *Path);
			Failed = true;
		}
		delete FileHandle;
		if (Failed)
		{
			bDownloadFailed = true;
		}
	}
	bDownloadFile1Completed = true;
	OnOneDownloadComplete();
}

void ASBSDownloadSubsystem::OnDownloadCompleteFile2(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!(bSuccess && Response.IsValid() && Response->GetResponseCode() == 200))
	{
		bDownloadFailed = true;
	}
	else
	{
		FString Path = GetCurrentBlueprintPath() + "/" + mCurrentDownload.OriginalName + ".sbpcfg";
		CheckFilePath(Path);
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		bool         Failed = false;
		IFileHandle* FileHandle = PlatformFile.OpenWrite(*Path);
		if (!FileHandle)
		{
			UE_LOG(LogSBS, Error, TEXT("Something went wrong while saving the file '%s'"), *Path);
			Failed = true;
		}

		if (!FileHandle->Write(Response->GetContent().GetData(), Response->GetContentLength()))
		{
			UE_LOG(LogSBS, Error, TEXT("Something went wrong while writing the response data to the file '%s'"), *Path);
			Failed = true;
		}
		delete FileHandle;
		if (Failed)
		{
			bDownloadFailed = true;
		}
	}
	bDownloadFile2Completed = true;
	OnOneDownloadComplete();
}

void ASBSDownloadSubsystem::OnDownloadProgressFile1(FHttpRequestPtr HttpRequest, int32 BytesSend,
	int32                                                           InBytesReceived) const
{
	int32            ReceivedSize = InBytesReceived;
	FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

	if (HttpResponse.IsValid())
	{
		int32 TotalSize = HttpResponse->GetContentLength();
		float PercentDone = (static_cast<float>(ReceivedSize) / static_cast<float>(TotalSize)) * 100;
		if (mOnDownloadProgress.IsBound())
		{
			mOnDownloadProgress.Broadcast(mCurrentDownload, PercentDone, TEXT(".sbp"));
		}
	}
}

void ASBSDownloadSubsystem::OnDownloadProgressFile2(FHttpRequestPtr HttpRequest, int32 BytesSend,
	int32                                                           InBytesReceived) const
{
	int32 ReceivedSize = InBytesReceived;

	FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

	if (HttpResponse.IsValid())
	{
		int32 TotalSize = HttpResponse->GetContentLength();
		float PercentDone = (static_cast<float>(ReceivedSize) / static_cast<float>(TotalSize)) * 100;
		if (mOnDownloadProgress.IsBound())
		{
			mOnDownloadProgress.Broadcast(mCurrentDownload, PercentDone, TEXT(".sbpcfg"));
		}
	}
}

void ASBSDownloadSubsystem::CheckFilePath(FString filePath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*GetCurrentBlueprintPath()))
	{
		if (!PlatformFile.CreateDirectoryTree(*GetCurrentBlueprintPath()))
		{
			UE_LOG(LogSBS, Error, TEXT("Unable to create a directory '%s' to save the downloaded file"),
				*GetCurrentBlueprintPath());
			return;
		}
	}

	if (FPaths::FileExists(*filePath))
	{
		IFileManager& FileManager = IFileManager::Get();
		if (!FileManager.Delete(*filePath))
		{
			UE_LOG(LogSBS, Error, TEXT("Something went wrong while deleting the existing file '%s'"), *filePath);
		}
	}
}


bool ASBSDownloadSubsystem::DownloadBlueprint(FBlueprintJsonStructure Blueprint)
{
	UE_LOG(LogSBS, Log, TEXT("Try to add Blueprint ID '%s' to queue"), *Blueprint.ID);
	if (!mDownloadQueueChecker.Contains(Blueprint) && mCurrentDownload != Blueprint)
	{
		UE_LOG(LogSBS, Warning, TEXT("Add Blueprint ID '%s' to queue"), *Blueprint.ID);
		mDownloadQueueChecker.EmplaceAt(0, Blueprint);
		return true;
	}
	return false;
}

bool ASBSDownloadSubsystem::DownloadBlueprintPack(FBlueprintPackJsonStructure BlueprintPack)
{
	for (FBlueprintInPackJsonStructure Blueprint : BlueprintPack.Blueprints)
	{
		FBlueprintJsonStructure Struc;
		Struc.ID = Blueprint.ID;
		Struc.Name = Blueprint.Name;
		Struc.OriginalName = Blueprint.OriginalName;
		DownloadBlueprint(Struc);
	}
	return true;
}

int32 ASBSDownloadSubsystem::GetDownloadStateForBlueprint(FBlueprintJsonStructure Blueprint)
{
	if (Blueprint == mCurrentDownload)
	{
		return 2;
	}
	if (mDownloadQueueChecker.Contains(Blueprint))
	{
		return 1;
	}
	return 0;
}

bool ASBSDownloadSubsystem::GetDownloadStateForBlueprintPack(FBlueprintPackJsonStructure BlueprintPack)
{
	for (FBlueprintInPackJsonStructure Blueprint : BlueprintPack.Blueprints)
	{
		FBlueprintJsonStructure Struc;
		Struc.ID = Blueprint.ID;
		Struc.Name = Blueprint.Name;
		Struc.OriginalName = Blueprint.OriginalName;
		if (mCurrentDownload == Struc || mDownloadQueueChecker.Contains(Struc))
		{
			return true;
		}
	}
	return false;
}

FString ASBSDownloadSubsystem::GetCurrentBlueprintPath()
{
	return mBlueprintSubsystem->GetSessionBlueprintPath();
}

bool ASBSDownloadSubsystem::IsBlueprintInstalled(FBlueprintJsonStructure Blueprint)
{
	FString Path = GetCurrentBlueprintPath() + "/" + Blueprint.OriginalName + ".sbp";
	FString PathCfg = GetCurrentBlueprintPath() + "/" + Blueprint.OriginalName + ".sbpcfg";
	return FPaths::FileExists(*Path) && FPaths::FileExists(*PathCfg);
}

bool ASBSDownloadSubsystem::OnBlueprintCreated_Implementation(UFGBlueprintDescriptor* BlueprintDescriptor,
	FBlueprintJsonStructure                                                           Blueprint)
{
	return false;
}