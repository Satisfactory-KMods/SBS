// Copyright Coffee Stain Studios. All Rights Reserved.


#include "../../Public/Structures/ApiPostStruct.h"

#include "Structures/ApiStatics.h"

void FApiPostStruct::MakeHeader( TMap< FString, FString >& Headers )
{
	Headers.Add( TEXT( "User-Agent" ), TEXT( "X-UnrealEngine-Agent" ) );
	Headers.Add( TEXT( "Content-Type" ), TEXT( "application/json" ) );
	Headers.Add( TEXT( "Accepts" ), TEXT( "application/json" ) );
	Headers.Add( TEXT( "x-api-key" ), FSBSStatics::API_KEY );
}

FString FApiPostStruct::ToString()
{
	FString out = "{}";
	TSharedPtr< FJsonObject > JsonObject = MakeShareable( new FJsonObject );

	ToJson( JsonObject );

	TSharedRef< TJsonWriter< > > Writer = TJsonWriterFactory< >::Create( &out );
	FJsonSerializer::Serialize( JsonObject.ToSharedRef(), Writer );

	return out;
}

void FFilterPostStruct::ToJson( TSharedPtr< FJsonObject >& JsonObject )
{
	FApiPostStruct::ToJson( JsonObject );

	TSharedPtr< FJsonObject > filterSchema = MakeShareable( new FJsonObject );
	TSharedPtr< FJsonObject > sortBy = MakeShareable( new FJsonObject );

	if( Tags.Num() > 0 )
	{
		TArray< TSharedPtr< FJsonValue > > TagValues;
		for( FString Tag : Tags )
		{
			TagValues.Add( MakeShareable( new FJsonValueString( Tag ) ) );
		}
		filterSchema->SetArrayField( "tags", TagValues );
	}

	if( Mods.Num() > 0 )
	{
		TArray< TSharedPtr< FJsonValue > > ModValues;
		for( FString Tag : Mods )
		{
			ModValues.Add( MakeShareable( new FJsonValueString( Tag ) ) );
		}
		filterSchema->SetArrayField( "mods", ModValues );
	}

	if( !SortByKey.IsEmpty() )
	{
		sortBy->SetStringField( "by", SortByKey );
		sortBy->SetBoolField( "up", SortByUp );
		filterSchema->SetObjectField( "sortBy", sortBy );
	}
	if( !Name.IsEmpty() )
	{
		filterSchema->SetStringField( "name", Name );
	}
	if( OnlyVanilla )
	{
		filterSchema->SetBoolField( "onlyVanilla", OnlyVanilla );
	}

	JsonObject->SetNumberField( "skip", Skip );
	JsonObject->SetNumberField( "limit", Limit );
	JsonObject->SetObjectField( "filterOptions", filterSchema );
}
