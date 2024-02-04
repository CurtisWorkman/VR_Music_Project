// Copyright Epic Games, Inc. All Rights Reserved.


#include "VR_CGameModeBase.h"
#include "InstrumentArea.h"

void AVR_CGameModeBase::StorePlayingInstrument(AInstrumentArea* Instrument)
{
	StoredInstrument = Instrument;
}

AInstrumentArea* AVR_CGameModeBase::GetStoredPlayingInstrument()
{
	return StoredInstrument;
}
