// Copyright Coffee Stain Studios. All Rights Reserved
#include "Subsystem/SBSApiSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "SBS/Public/Logging.h"
#include "Structures/ApiJsonStruct.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"

void USBSApiSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Collection.InitializeDependency( UKBFLAssetDataSubsystem::StaticClass() );
	Super::Initialize( Collection );

	UConfigProperty* Property = UKBFL_ConfigTools::GetConfigPropertyByKey( FSBSStatics::GETMODCONFIG(), "accountkey" );
	Property->OnPropertyValueChanged.AddUniqueDynamic( this, &USBSApiSubsystem::OnAccountKeyChanged );
	mOnDynamicQueryDone.AddUniqueDynamic( this, &USBSApiSubsystem::OnCheckKey );
	OnAccountKeyChanged();
}

void USBSApiSubsystem::QueryApi( FFilterPostStruct Post )
{
	if( bIsInQuery )
	{
		return;
	}

	const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL( Post.getUrl() );
	TMap< FString, FString > Headers;
	Post.MakeHeader( Headers );
	for( auto Header : Headers )
	{
		if( !Header.Key.IsEmpty() && !Header.Value.IsEmpty() )
		{
			Request->SetHeader( Header.Key, Header.Value );
		}
	}

	Request->SetVerb( "POST" );
	Request->SetContentAsString( Post.ToString() );
	Request->OnProcessRequestComplete().BindUObject( this, &USBSApiSubsystem::OnBlueprintQueryDone );
	Request->SetTimeout( 10 );
	if( Request->ProcessRequest() )
	{
		bIsInQuery = true;
	}
}

void USBSApiSubsystem::QueryApiDynamic( FDynamicApiPostStruct Post )
{
	mDynamicQuery = Post;
	if( bIsInQuery )
	{
		return;
	}

	const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL( Post.getUrl() );
	TMap< FString, FString > Headers;
	Post.MakeHeader( Headers );
	for( auto Header : Headers )
	{
		if( !Header.Key.IsEmpty() && !Header.Value.IsEmpty() )
		{
			Request->SetHeader( Header.Key, Header.Value );
		}
	}

	Request->SetVerb( "POST" );
	Request->SetContentAsString( Post.ToString() );
	Request->OnProcessRequestComplete().BindUObject( this, &USBSApiSubsystem::OnQueryDynamicDone );
	Request->SetTimeout( 10 );
	if( Request->ProcessRequest() )
	{
		bIsInQuery = true;
	}
}

void USBSApiSubsystem::QueryRating( FRatingPostStruct Post )
{
	if( !IsQuery() )
	{
		QueryApiDynamic( Post );
	}
}

bool USBSApiSubsystem::IsQuery() const
{
	return !mDynamicQuery.Indetifier.IsEmpty();
}

bool USBSApiSubsystem::HasAuth() const
{
	return bHasAuth;
}

inline USBSApiSubsystem* USBSApiSubsystem::Get( const UObject* WorldContext )
{
	if( IsValid( WorldContext ) )
	{
		return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem< USBSApiSubsystem >();
	}
	return nullptr;
}

void USBSApiSubsystem::OnBlueprintQueryDone( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess )
{
	bIsInQuery = false;
	bool success = false;
	if( bSuccess )
	{
		TSharedPtr< FJsonObject > JsonObject = MakeShareable( new FJsonObject );
		if( FJsonSerializer::Deserialize( TJsonReaderFactory< >::Create( Response->GetContentAsString() ), JsonObject ) )
		{
			const TArray< TSharedPtr< FJsonValue > >* DataArray;
			if( JsonObject->TryGetArrayField( "blueprints", DataArray ) )
			{
				success = JsonObject->TryGetNumberField( "totalBlueprints", mTotalBlueprints );
				mCurrentBlueprints.Empty();
				for( TSharedPtr< FJsonValue, ESPMode::NotThreadSafe > JsonValue : *DataArray )
				{
					FBlueprintJsonStructure Struct;
					Struct.setJsonObject( JsonValue->AsObject() );
					Struct.parse();
					mCurrentBlueprints.Add( Struct );
				}
			}
		}
	}

	if( mOnQueryDone.IsBound() )
	{
		mOnQueryDone.Broadcast( mCurrentBlueprints, mTotalBlueprints, success );
	}
}

void USBSApiSubsystem::OnQueryDynamicDone( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess )
{
	bIsInQuery = false;
	bool success = false;
	if( bSuccess )
	{
		TSharedPtr< FJsonObject > JsonObject = MakeShareable( new FJsonObject );
		if( FJsonSerializer::Deserialize( TJsonReaderFactory< >::Create( Response->GetContentAsString() ), JsonObject ) )
		{
			const bool error = JsonObject->TryGetStringField( "error", mDynamicQuery.FailedText );
			if( Response->GetResponseCode() == 200 )
			{
				success = !error;
			}
		}
	}
	else
	{
		mDynamicQuery.FailedText = "Failed to fetch API";
	}

	if( mOnQueryDone.IsBound() )
	{
		mOnDynamicQueryDone.Broadcast( mDynamicQuery, success );
	}
	mDynamicQuery.Indetifier.Empty();
}

void USBSApiSubsystem::OnAccountKeyChanged()
{
	FString AuthKey = FSBSStatics::GetAccountKey();
	if( !AuthKey.IsEmpty() )
	{
		FAuthCheckPostStruct Post;
		Post.Indetifier = "AuthCheck";
		QueryApiDynamic( Post );
	}
}

void USBSApiSubsystem::OnCheckKey( FDynamicApiPostStruct PostResult, bool Success )
{
	if( PostResult.Indetifier == FString( "AuthCheck" ) )
	{
		bHasAuth = Success;
	}
}
