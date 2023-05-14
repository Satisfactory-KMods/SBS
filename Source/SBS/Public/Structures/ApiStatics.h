// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "ApiStatics.generated.h"

USTRUCT()
struct SBS_API FSBSStatics
{
	GENERATED_BODY();

public:
	inline static FString API_KEY = "5d1cf871c14330001264924f7bcf50dc82c05e3f8fd26fa1d946c73a241042e16a43fe7b3f3d01b03c622b5908b7994bc2c70dd0c0b97414736354ec7e49ceeef8fa94c8834f6685da2";
	inline static FString API_URL = "https://sbs.kyrium.space/api/v1/";
	inline static FString API_URL_DEV = "http://127.0.0.1/api/v1/";
	
	inline static FString API_BLUEPRINT = "mod/getblueprints";
	
	static FString MakeUrl( FString To )
	{
		return API_URL.Append( To );
	};
};