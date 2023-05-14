// Copyright Coffee Stain Studios. All Rights Reserved
#include "Subsystem/SBSApiSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "SBS/Public/Logging.h"
#include "Structures/ApiJsonStruct.h"

void USBSApiSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );
	FFilterPostStruct Request;
	Request.Limit = 20;
	Request.Skip = 0;
	FOnBlueprintQueryDone OnQueryDone;
	QueryApi( Request, OnQueryDone );
}

void USBSApiSubsystem::QueryApi( FFilterPostStruct Request, FOnBlueprintQueryDone& OnQueryDone )
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindLambda( [this, OnQueryDone]( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess )
	{
		bool success = false;
		if ( bSuccess )
        {
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			if( FJsonSerializer::Deserialize( TJsonReaderFactory< >::Create( Response->GetContentAsString() ), JsonObject ) )
			{
				const TArray< TSharedPtr<FJsonValue> >* DataArray;
				if( JsonObject->TryGetArrayField( "blueprints", DataArray ) )
				{
					success = JsonObject->TryGetNumberField( "totalBlueprints", mTotalBlueprints );
					for (TSharedPtr<FJsonValue, ESPMode::NotThreadSafe> JsonValue : *DataArray )
					{
						FBlueprintJsonStructure Struct;
						Struct.setJsonObject( JsonValue->AsObject() );
						Struct.parse();
						mCurrentBlueprints.Add( Struct );
					}
				}
			}
        }
        else
        {
            UE_LOG( LogSBS, Error, TEXT( "QueryApi failed" ) );
        }
		OnQueryDone.ExecuteIfBound( mCurrentBlueprints, success );
	} );

	Request->SetURL( Request.getUrl() );
	for( auto Header : Headers )
	{
		if( Header.Key != "Accepts" && Header.Key != "Content-Type" && Header.Key != "User-Agent" )
		{
			if( !Header.Key.IsEmpty() && !Header.Value.IsEmpty() )
			{
				Request->SetHeader( Header.Key, Header.Value );
			}
		}
	}

	Request->SetHeader( TEXT( "User-Agent" ), TEXT( "X-UnrealEngine-Agent" ) );
	Request->SetHeader( TEXT( "Content-Type" ), TEXT( "application/json" ) );
	Request->SetHeader( TEXT( "Accepts" ), TEXT( "application/json" ) );

	Request->SetVerb( "POST" );
	Request->SetContentAsString( Request.ToString() );
	Request->ProcessRequest();
}

inline USBSApiSubsystem* USBSApiSubsystem::Get( const UObject* WorldContext )
{
	if( IsValid( WorldContext ) )
	{
		return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem< USBSApiSubsystem >();
	}
	return nullptr;
}
