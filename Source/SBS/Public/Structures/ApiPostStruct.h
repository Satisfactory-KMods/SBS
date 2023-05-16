// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "ApiStatics.h"
#include "ApiPostStruct.generated.h"

USTRUCT( BlueprintType )
struct SBS_API FApiPostStruct
{
	GENERATED_BODY()
	;

	virtual ~FApiPostStruct()
	{
	}

public:
	// create Header
	static void MakeHeader( TMap< FString, FString >& Headers );

	// make Struct to a JSON object string
	FString ToString();

	virtual FString getUrl()
	{
		return FSBSStatics::MakeUrl( "" );
	}

protected:
	// write post data to JSON Object
	virtual void ToJson( TSharedPtr< FJsonObject >& JsonObject )
	{
	}
};

USTRUCT( BlueprintType )
struct SBS_API FDynamicApiPostStruct : public FApiPostStruct
{
	GENERATED_BODY()
	;

	virtual ~FDynamicApiPostStruct() override
	{
	}

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
	FText NotifyText = FText::FromString( "" );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
	FString Indetifier = FString();

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
	FString FailedText = FString();
};

USTRUCT( BlueprintType )
struct SBS_API FAuthCheckPostStruct : public FDynamicApiPostStruct
{
	GENERATED_BODY()
	;

	virtual FString getUrl() override
	{
		return FSBSStatics::MakeUrl( FSBSStatics::API_AUTH );
	};
};

USTRUCT( BlueprintType )
struct SBS_API FFilterPostStruct : public FApiPostStruct
{
	GENERATED_BODY()
	;

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

	virtual FString getUrl() override
	{
		return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINT );
	};

protected:
	virtual void ToJson( TSharedPtr< FJsonObject >& JsonObject ) override;
};

USTRUCT( BlueprintType )
struct SBS_API FRatingPostStruct : public FDynamicApiPostStruct
{
	GENERATED_BODY()
	;

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rating Post Struct" )
	FString BlueprintID = "";

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rating Post Struct" )
	int32 Rating = 1;

	virtual FString getUrl() override
	{
		return FSBSStatics::MakeUrl( FSBSStatics::API_RATEBP );
	};

protected:
	virtual void ToJson( TSharedPtr< FJsonObject >& JsonObject ) override;
};

USTRUCT()
struct SBS_API FDownloadSbpStruct : public FApiPostStruct
{
	GENERATED_BODY()
	;

	FString ID = "";

	virtual FString getUrl() override
	{
		return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINTDOWNLOAD + ID + "/sbp" );
	};
};

USTRUCT()
struct SBS_API FDownloadSbpcfgStruct : public FDownloadSbpStruct
{
	GENERATED_BODY()
	;

	virtual FString getUrl() override
	{
		return FSBSStatics::MakeUrl( FSBSStatics::API_BLUEPRINTDOWNLOAD + ID + "/sbpcfg" );
	};
};
