// Copyright Coffee Stain Studios. All Rights Reserved.

#include "Structures/ApiJsonStruct.h"

void FBlueprintJsonStructure::parse()
{
	FApiJsonStruct::parse();

	if( mJsonObject )
	{
		mJsonObject->TryGetStringField( "_id", ID );
		mJsonObject->TryGetStringField( "owner", Owner );
		mJsonObject->TryGetStringField( "DesignerSize", DesignerSize );
		mJsonObject->TryGetStringField( "name", Name );
		mJsonObject->TryGetStringField( "createdAt", CreatedAt );
		mJsonObject->TryGetStringField( "updatedAt", UpdatedAt );
		mJsonObject->TryGetStringField( "updatedAt", UpdatedAt );
		mJsonObject->TryGetStringField( "description", Description );
		mJsonObject->TryGetStringField( "originalName", OriginalName );
		mJsonObject->TryGetStringArrayField( "mods", Mods );
		mJsonObject->TryGetStringArrayField( "images", Images );
		mJsonObject->TryGetNumberField( "totalRating", TotalRating );
		mJsonObject->TryGetNumberField( "totalRatingCount", TotalRatingCount );

		const TArray< TSharedPtr< FJsonValue > >* DataArray;
		if( mJsonObject->TryGetArrayField( "tags", DataArray ) )
		{
			for( TSharedPtr< FJsonValue, ESPMode::NotThreadSafe > JsonValue : *DataArray )
			{
				FBlueprintJsonTagStructure Struct;
				Struct.setJsonObject( JsonValue->AsObject() );
				Struct.parse();
				Tags.Add( Struct );
			}
		}

		const TSharedPtr< FJsonObject >* Object;
		if( mJsonObject->TryGetObjectField( "iconData", Object ) )
		{
			IconData.mJsonObject = *Object;
			IconData.parse();
		}
	}
}

void FBlueprintJsonTagStructure::parse()
{
	FApiJsonStruct::parse();

	if( mJsonObject )
	{
		mJsonObject->TryGetStringField( "_id", ID );
		mJsonObject->TryGetStringField( "DisplayName", DisplayName );
	}
}

void FBlueprintJsonColorStructure::parse()
{
	FApiJsonStruct::parse();

	if( mJsonObject )
	{
		const TSharedPtr< FJsonObject >* Object;
		if( mJsonObject->TryGetObjectField( "color", Object ) )
		{
			TSharedPtr< FJsonObject > Json = *Object;

			double a, r, g, b;

			Json->TryGetNumberField( "a", a );
			Json->TryGetNumberField( "r", r );
			Json->TryGetNumberField( "g", g );
			Json->TryGetNumberField( "b", b );

			Color = FLinearColor( r, g, b, a );
		}
		mJsonObject->TryGetNumberField( "iconID", IconID );
	}
}
