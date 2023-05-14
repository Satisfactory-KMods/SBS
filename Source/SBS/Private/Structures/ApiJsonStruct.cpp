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
		mJsonObject->TryGetStringArrayField( "tags", Tags );
		mJsonObject->TryGetStringArrayField( "images", Images );
		mJsonObject->TryGetNumberField( "totalRating", TotalRating );
		mJsonObject->TryGetNumberField( "totalRatingCount", TotalRatingCount );
	}
}