// Copyright Coffee Stain Studios. All Rights Reserved.

#include "Structures/ApiJsonStruct.h"

void FBlueprintJsonStructure::parse(UObject* WorldContext)
{
	if (mJsonObject)
	{
		mJsonObject->TryGetStringField("_id", ID);
		mJsonObject->TryGetStringField("owner", Owner);
		mJsonObject->TryGetStringField("DesignerSize", DesignerSize);
		mJsonObject->TryGetStringField("name", Name);
		mJsonObject->TryGetStringField("createdAt", CreatedAt);
		mJsonObject->TryGetStringField("updatedAt", UpdatedAt);
		mJsonObject->TryGetStringField("updatedAt", UpdatedAt);
		mJsonObject->TryGetStringField("originalName", OriginalName);
		mJsonObject->TryGetStringArrayField("mods", Mods);
		mJsonObject->TryGetStringArrayField("images", Images);
		mJsonObject->TryGetNumberField("totalRating", TotalRating);
		mJsonObject->TryGetNumberField("totalRatingCount", TotalRatingCount);
		mJsonObject->TryGetNumberField("downloads", Downloads);
		mJsonObject->TryGetStringField("SCIM", SCIM);

		const TArray<TSharedPtr<FJsonValue>>* DataArray;
		if (mJsonObject->TryGetArrayField("tags", DataArray))
		{
			for (auto JsonValue : *DataArray)
			{
				FBlueprintJsonTagStructure Struct;
				Struct.setJsonObject(JsonValue->AsObject());
				Struct.parse(WorldContext);
				Tags.Add(Struct);
			}
		}

		const TSharedPtr<FJsonObject>* Object;
		if (mJsonObject->TryGetObjectField("iconData", Object))
		{
			IconData.mJsonObject = *Object;
			IconData.parse(WorldContext);
		}
	}
}

void FBlueprintInPackJsonStructure::parse(UObject* WorldContext)
{
	FApiJsonStruct::parse(WorldContext);
	const TSharedPtr<FJsonObject>* Object;

	mJsonObject->TryGetStringField("_id", ID);
	mJsonObject->TryGetStringField("originalName", OriginalName);
	mJsonObject->TryGetStringField("name", Name);

	if (mJsonObject->TryGetObjectField("iconData", Object))
	{
		IconData.mJsonObject = *Object;
		IconData.parse(WorldContext);
	}
}

void FBlueprintPackJsonStructure::parse(UObject* WorldContext)
{
	FApiJsonStruct::parse(WorldContext);
	if (mJsonObject)
	{
		mJsonObject->TryGetStringField("_id", ID);
		mJsonObject->TryGetStringField("owner", Owner);
		mJsonObject->TryGetStringField("name", Name);
		mJsonObject->TryGetStringField("createdAt", CreatedAt);
		mJsonObject->TryGetStringField("updatedAt", UpdatedAt);
		mJsonObject->TryGetStringField("updatedAt", UpdatedAt);
		mJsonObject->TryGetStringArrayField("mods", Mods);
		mJsonObject->TryGetNumberField("totalRating", TotalRating);
		mJsonObject->TryGetNumberField("totalRatingCount", TotalRatingCount);

		const TArray<TSharedPtr<FJsonValue>>* BPDataArray;
		if (mJsonObject->TryGetArrayField("blueprints", BPDataArray))
		{
			for (auto JsonValue : *BPDataArray)
			{
				FBlueprintInPackJsonStructure Struct;
				Struct.setJsonObject(JsonValue->AsObject());
				Struct.parse(WorldContext);
				Blueprints.Add(Struct);
			}
		}

		FString Image;
		mJsonObject->TryGetStringField("image", Image);
		ImageUrl = FSBSStatics::MakeUrl("image/" + Blueprints[0].ID + "/" + Image, WorldContext);
		FirstIconData = Blueprints[0].IconData;

		const TArray<TSharedPtr<FJsonValue>>* DataArray;
		if (mJsonObject->TryGetArrayField("tags", DataArray))
		{
			for (auto JsonValue : *DataArray)
			{
				FBlueprintJsonTagStructure Struct;
				Struct.setJsonObject(JsonValue->AsObject());
				Struct.parse(WorldContext);
				Tags.Add(Struct);
			}
		}
	}
}

void FBlueprintJsonTagStructure::parse(UObject* WorldContext)
{
	FApiJsonStruct::parse(WorldContext);

	if (mJsonObject)
	{
		mJsonObject->TryGetStringField("_id", ID);
		mJsonObject->TryGetStringField("DisplayName", DisplayName);
	}
}

void FBlueprintJsonColorStructure::parse(UObject* WorldContext)
{
	FApiJsonStruct::parse(WorldContext);

	if (mJsonObject)
	{
		const TSharedPtr<FJsonObject>* Object;
		if (mJsonObject->TryGetObjectField("color", Object))
		{
			TSharedPtr<FJsonObject> Json = *Object;

			double a, r, g, b;

			Json->TryGetNumberField("a", a);
			Json->TryGetNumberField("r", r);
			Json->TryGetNumberField("g", g);
			Json->TryGetNumberField("b", b);

			Color = FLinearColor(r, g, b, a);
		}
		mJsonObject->TryGetNumberField("iconID", IconID);
	}
}