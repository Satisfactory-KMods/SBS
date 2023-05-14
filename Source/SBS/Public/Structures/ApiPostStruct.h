// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "ApiStatics.h"
#include "CoreMinimal.h"
#include "ApiPostStruct.generated.h"

USTRUCT(BlueprintType)
struct SBS_API FApiPostStruct
{
	GENERATED_BODY();
    virtual ~FApiPostStruct() {}
 
public:
	// create Header
	static void MakeHeader(TMap< FString, FString >& Headers);

	// make Struct to a JSON object string
	FString ToString();
	
	virtual FString getUrl()
	{
		return FSBSStatics::MakeUrl("");
	}

protected:
	// write post data to JSON Object
	virtual void ToJson( TSharedPtr<FJsonObject>& JsonObject ) {}
};

USTRUCT(BlueprintType)
struct SBS_API FFilterPostStruct : public FApiPostStruct
{
	GENERATED_BODY();
 
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
	TArray<FString> Tags;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
	TArray<FString> Mods;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Filter Post Struct" )
	bool OnlyVanilla = false;
	
	virtual FString getUrl() override
	{
		return FSBSStatics::MakeUrl(FSBSStatics::API_BLUEPRINT);
	};
	
protected:
	virtual void ToJson( TSharedPtr<FJsonObject>& JsonObject ) override;
};