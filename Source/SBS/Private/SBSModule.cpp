#include "SBSModule.h"

#include "FGGameMode.h"
#include "Equipment/FGHoverPack.h"
#include "Network/KPCLNetworkConnectionComponent.h"
#include "Patching/NativeHookManager.h"
#include "Replication/SBSDefaultRCO.h"

/*void GameModePostLogin( CallScope< void( * )( AFGGameMode*, APlayerController* ) >& scope, AFGGameMode* gm, APlayerController* pc ) {
	if( gm->HasAuthority( ) && !gm->IsMainMenuGameMode( ) ) {
		//Register RCO
		gm->RegisterRemoteCallObjectClass( USBSDefaultRCO::StaticClass( ) );
	}
}*/

void FSBSModule::StartupModule()
{
	#if !WITH_EDITOR
	// Hooking to register RCOs
	//AFGGameMode* LocalGameMode = GetMutableDefault<AFGGameMode>();
	//SUBSCRIBE_METHOD_VIRTUAL( AFGGameMode::PostLogin, LocalGameMode, &GameModePostLogin );
	#endif
}

IMPLEMENT_GAME_MODULE(FSBSModule, SBS);