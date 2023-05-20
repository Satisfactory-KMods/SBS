﻿#pragma once
#include "CoreMinimal.h"
#include "ApiStatics.h"
#include "ApiJsonStruct.generated.h"

USTRUCT( BlueprintType )
struct SBS_API FApiJsonStruct {
	GENERATED_BODY( )
	;

	virtual ~FApiJsonStruct( ) { }

	public:
		TSharedPtr< FJsonObject > mJsonObject;

		void setJsonObject( TSharedPtr< FJsonObject > object ) {
			mJsonObject = object;
		}

		virtual void parse( ) { }
};

USTRUCT( BlueprintType )
struct SBS_API FBlueprintJsonTagStructure : public FApiJsonStruct {
	GENERATED_BODY( )
	;

	public:
		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString ID;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString DisplayName;

		virtual void parse( ) override;
};

USTRUCT( BlueprintType )
struct SBS_API FBlueprintJsonColorStructure : public FApiJsonStruct {
	GENERATED_BODY( )
	;

	public:
		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		int32 IconID;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FLinearColor Color;

		virtual void parse( ) override;
};

USTRUCT( BlueprintType )
struct SBS_API FBlueprintJsonStructure : public FApiJsonStruct {
	GENERATED_BODY( )
	;

	public:
		bool operator==( const FBlueprintJsonStructure& Other ) const {
			return ID == Other.ID;
		}

		bool operator!=( const FBlueprintJsonStructure& Other ) const {
			return ID != Other.ID;
		}

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString ID;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		TArray< FString > Mods;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		TArray< FBlueprintJsonTagStructure > Tags;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString Owner;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString DesignerSize;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString Name;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString CreatedAt;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString UpdatedAt;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		int32 Downloads;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		int32 TotalRating;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		int32 TotalRatingCount;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		TArray< FString > Images;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString OriginalName;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FBlueprintJsonColorStructure IconData;

		virtual void parse( ) override;
};


USTRUCT( BlueprintType )
struct SBS_API FBlueprintInPackJsonStructure : public FApiJsonStruct {
	GENERATED_BODY( )
	;

	public:
	bool operator==( const FBlueprintJsonStructure& Other ) const {
		return ID == Other.ID;
	}

	bool operator!=( const FBlueprintJsonStructure& Other ) const {
		return ID != Other.ID;
	}

	UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
	FString ID;

	UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
	FString Name;

	UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
	FString OriginalName;

	UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
	FBlueprintJsonColorStructure IconData;

	virtual void parse( ) override;
};


USTRUCT( BlueprintType )
struct SBS_API FBlueprintPackJsonStructure : public FApiJsonStruct {
	GENERATED_BODY( )
	;

	public:
		bool operator==( const FBlueprintJsonStructure& Other ) const {
			return ID == Other.ID;
		}

		bool operator!=( const FBlueprintJsonStructure& Other ) const {
			return ID != Other.ID;
		}

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString ID;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		TArray< FString > Mods;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		TArray< FBlueprintJsonTagStructure > Tags;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString Owner;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString Name;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString CreatedAt;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		FString UpdatedAt;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		int32 TotalRating;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		int32 TotalRatingCount;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
		TArray< FBlueprintInPackJsonStructure > Blueprints;

		UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
	FString ImageUrl;

	UPROPERTY( BlueprintReadOnly, Category = "BlueprintJsonStructure" )
	FBlueprintJsonColorStructure FirstIconData;

		virtual void parse( ) override;
};
