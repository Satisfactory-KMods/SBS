// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "ApiStatics.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "ApiPostStruct.generated.h"

USTRUCT( BlueprintType )
struct SBS_API FApiPostStruct {
	GENERATED_BODY( )
	;

	virtual ~FApiPostStruct( ) { }

	public:
		// create Header
		static void MakeHeader( TMap< FString, FString >& Headers );

		// make Struct to a JSON object string
		FString ToString( );

		virtual FString getUrl( ) {
			return FSBSStatics::MakeUrl( "" );
		}

	protected:
		// write post data to JSON Object
		virtual void ToJson( TSharedPtr< FJsonObject >& JsonObject ) { }
};

USTRUCT( BlueprintType )
struct SBS_API FDynamicApiPostStruct : public FApiPostStruct {
	GENERATED_BODY( )
	;

	virtual ~FDynamicApiPostStruct( ) override { }

	bool operator==( const FDynamicApiPostStruct& Other ) const {
		return Indetifier == Other.Indetifier;
	}

	bool operator!=( const FDynamicApiPostStruct& Other ) const {
		return Indetifier != Other.Indetifier;
	}

	public:
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		FText NotifyText = FText::FromString( "" );

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		FString Indetifier = FString( );

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		FString FailedText = FString( );

		virtual void OnRequestDone( FHttpRequestPtr Request, FHttpResponsePtr Response, class USBSApiSubsystem* Subsystem, TSharedPtr< FJsonObject > JsonObject );
};

USTRUCT( BlueprintType )
struct SBS_API FAuthCheckPostStruct : public FDynamicApiPostStruct {
	GENERATED_BODY( )
	;

	virtual FString getUrl( ) override {
		return FSBSStatics::MakeUrl( FSBSStatics::API_AUTH );
	};
};

USTRUCT( BlueprintType )
struct SBS_API FFilterPostStruct : public FApiPostStruct {
	GENERATED_BODY( )
	;

	virtual ~FFilterPostStruct( ) override { }

	public:
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		int32 Limit = 20;

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		int32 Skip = 0;

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		FString SortByKey = "";

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		bool SortByUp = false;

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		FString Name = "";

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		TArray< FString > Tags;

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		TArray< FString > Mods;

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
		bool OnlyVanilla = false;

		virtual FString getUrl( ) override {
			return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINT );
		};

	protected:
		virtual void ToJson( TSharedPtr< FJsonObject >& JsonObject ) override;
};

USTRUCT( BlueprintType )
struct SBS_API FPackFilterPostStruct : public FFilterPostStruct {
	GENERATED_BODY( )

	virtual FString getUrl( ) override {
		return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINTPACKS );
	};
};

USTRUCT( BlueprintType )
struct SBS_API FRatingPostStruct : public FDynamicApiPostStruct {
	GENERATED_BODY( )
	;

	public:
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rating Post Struct" )
		FString BlueprintID = "";

		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rating Post Struct" )
		int32 Rating = 1;

		virtual FString getUrl( ) override {
			return FSBSStatics::MakeUrl( FSBSStatics::API_RATEBP );
		};

	protected:
		virtual void ToJson( TSharedPtr< FJsonObject >& JsonObject ) override;
};

USTRUCT( )
struct SBS_API FDownloadSbpStruct : public FApiPostStruct {
	GENERATED_BODY( )
	;

	FString ID = "";

	virtual FString getUrl( ) override {
		return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINTDOWNLOAD + ID + "/sbp" );
	};
};

USTRUCT( )
struct SBS_API FDownloadSbpcfgStruct : public FDownloadSbpStruct {
	GENERATED_BODY( )
	;

	virtual FString getUrl( ) override {
		return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINTDOWNLOAD + ID + "/sbpcfg" );
	};
};

USTRUCT( BlueprintType )
struct SBS_API FSBSUserData : public FDynamicApiPostStruct {
	GENERATED_BODY( )

	public:
		UPROPERTY( BlueprintReadOnly )
		FString ID = "";

		UPROPERTY( BlueprintReadOnly )
		FString Username = "";

		UPROPERTY( BlueprintReadOnly )
		int32 Role = -1;

		virtual FString getUrl( ) override {
			return FSBSStatics::MakeUrl( FSBSStatics::API_AUTH );
		};
};

USTRUCT( BlueprintType )
struct SBS_API FGetTagsStruct : public FDynamicApiPostStruct {
	GENERATED_BODY( )
	;

	virtual FString getUrl( ) override {
		return FSBSStatics::MakeUrl( FSBSStatics::API_TAGS );
	};

	virtual void OnRequestDone( FHttpRequestPtr Request, FHttpResponsePtr Response, class USBSApiSubsystem* Subsystem, TSharedPtr< FJsonObject > JsonObject ) override;
};
