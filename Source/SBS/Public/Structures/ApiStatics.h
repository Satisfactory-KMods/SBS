// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "FGLocalPlayer.h"
#include "FGNetworkLibrary.h"
#include "FGPlayerState.h"
#include "FGSaveSession.h"
#include "FGSaveSystem.h"
#include "BFL/KBFL_ConfigTools.h"
#include "BFL/KBFL_Player.h"
#include "Configuration/ModConfiguration.h"
#include "ApiStatics.generated.h"

USTRUCT()
struct SBS_API FSBSStatics
{
	GENERATED_BODY()
	;

public:
	// deprecated but needed until the frontend and api is updated.
	// @note: this api key is not secret or something like this. since the download and endpoint is public.
	inline static FString API_KEY =
		"5d1cf871c14330001264924f7bcf50dc82c05e3f8fd26fa1d946c73a241042e16a43fe7b3f3d01b03c622b5908b7994bc2c70dd0c0b97414736354ec7e49ceeef8fa94c8834f6685da2";

	inline static FString API_URL = "https://sbs.kmods.space/api/v1/";
	inline static FString API_URL_LOCALDEV = "http://127.0.0.1/api/v1/";

	// deprecated 
	inline static FString API_URL_DEV = "https://dev-sbs.kmods.space/api/v1/";
	
	inline static FString API_AUTH = "mod/authcheck";
	inline static FString API_BLUEPRINT = "mod/getblueprints";
	inline static FString API_BLUEPRINTPACKS = "mod/getblueprintpacks";
	inline static FString API_TAGS = "mod/gettags";
	inline static FString API_RATEBP = "mod/rateblueprint";
	inline static FString API_BLUEPRINTDOWNLOAD = "download/";

	inline static TSubclassOf<UModConfiguration> MODCONFIG = nullptr;

	static TSubclassOf<UModConfiguration> GETMODCONFIG()
	{
		if (!MODCONFIG)
		{
			MODCONFIG = LoadClass<UModConfiguration>(nullptr, TEXT("/SBS/SBS_ModConfig.SBS_ModConfig_C"));
		}
		return MODCONFIG;
	}

	static FString MakeUrl(FString To, UObject* WorldContext)
	{
		GETMODCONFIG();
		bool isDev = UKBFL_ConfigTools::GetBoolFromConfig(MODCONFIG, "usedev", WorldContext);
		bool isLocalDev = UKBFL_ConfigTools::GetBoolFromConfig(MODCONFIG, "uselocaldev", WorldContext);
		if (isLocalDev)
		{
			return API_URL_LOCALDEV + To;
		}
		if (isDev)
		{
			return API_URL_DEV + To;
		}
		return API_URL + To;
	};

	static FString GetAccountKey(UObject* WorldContext)
	{
		GETMODCONFIG();
		return UKBFL_ConfigTools::GetStringFromConfig(MODCONFIG, "accountkey", WorldContext);
	};
};
