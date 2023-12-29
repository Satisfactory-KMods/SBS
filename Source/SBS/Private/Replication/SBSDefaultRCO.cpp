// Copyright Coffee Stain Studios. All Rights Reserved.
#include "Replication/SBSDefaultRCO.h"

#include "Net/UnrealNetwork.h"

void USBSDefaultRCO::GetLifetimeReplicatedProps( TArray< FLifetimeProperty >& OutLifetimeProps ) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( USBSDefaultRCO, mDummy2 );
}
