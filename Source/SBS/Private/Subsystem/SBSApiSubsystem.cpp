// Copyright Coffee Stain Studios. All Rights Reserved
#include "Subsystem/SBSApiSubsystem.h"

#include "HttpModule.h"

#include "SBS/Public/Logging.h"
#include "Structures/ApiJsonStruct.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"

void USBSApiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UKBFLAssetDataSubsystem::StaticClass());
	Super::Initialize(Collection);

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &USBSApiSubsystem::Tick), 1);

	UConfigProperty* Property = UKBFL_ConfigTools::GetConfigPropertyByKey(
		FSBSStatics::GETMODCONFIG(), "accountkey", GetWorld());
	Property->OnPropertyValueChanged.AddUniqueDynamic(this, &USBSApiSubsystem::OnAccountKeyChanged);
	OnAccountKeyChanged();
}

void USBSApiSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	Super::Deinitialize();
}

bool USBSApiSubsystem::Tick(float DeltaSeconds)
{
	if (!IsQuery() && mDynamicQueue.Num() > 0)
	{
		mDynamicQuery = mDynamicQueue.Pop();

		const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(mDynamicQuery.getUrl(GetWorld()));
		TMap<FString, FString> Headers;
		mDynamicQuery.MakeHeader(Headers, GetWorld());
		for (auto Header : Headers)
		{
			if (!Header.Key.IsEmpty() && !Header.Value.IsEmpty())
			{
				Request->SetHeader(Header.Key, Header.Value);
			}
		}

		Request->SetVerb("POST");
		Request->SetContentAsString(mDynamicQuery.ToString());
		Request->OnProcessRequestComplete().BindUObject(this, &USBSApiSubsystem::OnQueryDynamicDone);
		Request->SetTimeout(10);
		if (Request->ProcessRequest())
		{
			bIsInQuery = true;
		}
	}

	return true;
}

void USBSApiSubsystem::QueryApi(FFilterPostStruct Post)
{
	if (bIsInQuery)
	{
		return;
	}

	const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Post.getUrl(GetWorld()));
	TMap<FString, FString> Headers;
	Post.MakeHeader(Headers, GetWorld());
	for (auto Header : Headers)
	{
		if (!Header.Key.IsEmpty() && !Header.Value.IsEmpty())
		{
			Request->SetHeader(Header.Key, Header.Value);
		}
	}

	Request->SetVerb("POST");
	Request->SetContentAsString(Post.ToString());
	Request->OnProcessRequestComplete().BindUObject(this, &USBSApiSubsystem::OnBlueprintQueryDone);
	Request->SetTimeout(10);
	if (Request->ProcessRequest())
	{
		bIsInQuery = true;
	}
}

void USBSApiSubsystem::QueryPackApi(FPackFilterPostStruct Post)
{
	if (bIsInPackQuery)
	{
		return;
	}

	const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Post.getUrl(GetWorld()));
	TMap<FString, FString> Headers;
	Post.MakeHeader(Headers, GetWorld());
	for (auto Header : Headers)
	{
		if (!Header.Key.IsEmpty() && !Header.Value.IsEmpty())
		{
			Request->SetHeader(Header.Key, Header.Value);
		}
	}

	Request->SetVerb("POST");
	Request->SetContentAsString(Post.ToString());
	Request->OnProcessRequestComplete().BindUObject(this, &USBSApiSubsystem::OnBlueprintPackQueryDone);
	Request->SetTimeout(10);
	if (Request->ProcessRequest())
	{
		bIsInPackQuery = true;
	}
}

void USBSApiSubsystem::QueryApiDynamic(FDynamicApiPostStruct Post)
{
	mDynamicQueue.AddUnique(Post);
}

void USBSApiSubsystem::QueryRating(FRatingPostStruct Post)
{
	if (!IsQuery())
	{
		QueryApiDynamic(Post);
	}
}

bool USBSApiSubsystem::IsQuery() const
{
	return !mDynamicQuery.Indetifier.IsEmpty();
}

bool USBSApiSubsystem::IsPackQuery() const
{
	return bIsInQuery;
}

bool USBSApiSubsystem::IsBpQuery() const
{
	return bIsInPackQuery;
}

bool USBSApiSubsystem::IsAuthQuery() const
{
	return bIsInAuthQuery;
}

void USBSApiSubsystem::QueryForAuth()
{
	if (bIsInAuthQuery)
	{
		return;
	}

	const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(mUserData.getUrl(GetWorld()));
	TMap<FString, FString> Headers;
	FApiPostStruct::MakeHeader(Headers, GetWorld());
	for (auto Header : Headers)
	{
		if (!Header.Key.IsEmpty() && !Header.Value.IsEmpty())
		{
			Request->SetHeader(Header.Key, Header.Value);
		}
	}

	Request->SetVerb("POST");
	Request->SetContentAsString("{}");
	Request->OnProcessRequestComplete().BindUObject(this, &USBSApiSubsystem::OnQueryDynamicDone);
	Request->SetTimeout(10);
	if (Request->ProcessRequest())
	{
		bIsInAuthQuery = true;
	}
}

inline USBSApiSubsystem* USBSApiSubsystem::Get(const UObject* WorldContext)
{
	if (IsValid(WorldContext))
	{
		return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem<USBSApiSubsystem>();
	}
	return nullptr;
}

void USBSApiSubsystem::OnBlueprintQueryDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	bIsInQuery = false;
	bool success = false;
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response->GetContentAsString()), JsonObject))
		{
			const TArray<TSharedPtr<FJsonValue>>* DataArray;
			if (JsonObject->TryGetArrayField("blueprints", DataArray))
			{
				success = JsonObject->TryGetNumberField("totalBlueprints", mTotalBlueprints);
				mCurrentBlueprints.Empty();
				for (auto JsonValue : *DataArray)
				{
					FBlueprintJsonStructure Struct;
					Struct.setJsonObject(JsonValue->AsObject());
					Struct.parse(GetWorld());
					mCurrentBlueprints.Add(Struct);
				}
			}
		}
	}

	if (mOnQueryDone.IsBound())
	{
		mOnQueryDone.Broadcast(mCurrentBlueprints, mTotalBlueprints, success);
	}
}

void USBSApiSubsystem::OnBlueprintPackQueryDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	bIsInPackQuery = false;
	bool success = false;
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response->GetContentAsString()), JsonObject))
		{
			const TArray<TSharedPtr<FJsonValue>>* DataArray;
			if (JsonObject->TryGetArrayField("blueprints", DataArray))
			{
				success = JsonObject->TryGetNumberField("totalBlueprints", mTotalBlueprintPacks);
				mCurrentBlueprintPacks.Empty();
				for (auto JsonValue : *DataArray)
				{
					FBlueprintPackJsonStructure Struct;
					Struct.setJsonObject(JsonValue->AsObject());
					Struct.parse(GetWorld());
					mCurrentBlueprintPacks.Add(Struct);
				}
			}
		}
	}

	if (mOnPackQueryDone.IsBound())
	{
		mOnPackQueryDone.Broadcast(mCurrentBlueprintPacks, mTotalBlueprintPacks, success);
	}
}

void USBSApiSubsystem::OnQueryDynamicDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	bIsInQuery = false;
	bool success = false;
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response->GetContentAsString()), JsonObject))
		{
			const bool error = JsonObject->TryGetStringField("error", mDynamicQuery.FailedText);
			if (Response->GetResponseCode() == 200)
			{
				success = !error;
				if (success)
				{
					mDynamicQuery.OnRequestDone(Request, Response, this, JsonObject, GetWorld());
				}
			}
		}
	}
	else
	{
		mDynamicQuery.FailedText = "Failed to fetch API";
	}

	if (mOnDynamicQueryDone.IsBound())
	{
		mOnDynamicQueryDone.Broadcast(mDynamicQuery, success);
	}
	mDynamicQuery.Indetifier.Empty();
}

void USBSApiSubsystem::OnQueryAuthDone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	bIsInAuthQuery = false;
	bool success = false;
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response->GetContentAsString()), JsonObject))
		{
			const bool error = JsonObject->TryGetStringField("error", mDynamicQuery.FailedText);
			if (Response->GetResponseCode() == 200)
			{
				success = !error;
			}
			if (!error)
			{
				if (!JsonObject->TryGetNumberField(TEXT("role"), mUserData.Role))
				{
					mUserData.Role = -1;
				}
				else
				{
					JsonObject->TryGetStringField(TEXT("_id"), mUserData.ID);
					JsonObject->TryGetStringField(TEXT("username"), mUserData.Username);
				}
			}
			else
			{
				mUserData.Role = -1;
			}
		}
		else
		{
			mUserData.Role = -1;
		}
	}
	else
	{
		mUserData.Role = -1;
	}

	if (mOnAuthUpdated.IsBound())
	{
		mOnAuthUpdated.Broadcast(mUserData, success);
	}
}

void USBSApiSubsystem::OnAccountKeyChanged()
{
	FString AuthKey = FSBSStatics::GetAccountKey(GetWorld());
	if (!AuthKey.IsEmpty())
	{
		QueryForAuth();
	}
}
