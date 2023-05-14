﻿#pragma once
#include "ApiStatics.h"
#include "CoreMinimal.h"
#include "ApiJsonStruct.generated.h"

USTRUCT(BlueprintType)
struct SBS_API FApiJsonStruct
{
	GENERATED_BODY();

public:
	TSharedPtr<FJsonObject> mJsonObject;

	void setJsonObject(TSharedPtr<FJsonObject> object)
	{
		mJsonObject = object;
	}

	virtual void parse() {}
};

USTRUCT(BlueprintType)
struct SBS_API FBlueprintJsonStructure : public FApiJsonStruct
{
	GENERATED_BODY();

public:
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString ID;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	TArray<FString> Mods;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	TArray<FString> Tags;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString Owner;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString DesignerSize;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString Name;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString CreatedAt;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString UpdatedAt;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString Description;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	int32 Downloads;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	int32 TotalRating;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	int32 TotalRatingCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	TArray<FString> Images;
	
	UPROPERTY(BlueprintReadOnly, Category = "BlueprintJsonStructure")
	FString OriginalName;
	
	virtual void parse() override;
};