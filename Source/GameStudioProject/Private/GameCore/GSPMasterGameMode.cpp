	// Game Studio Project Team F 2023


#include "GameCore/GSPMasterGameMode.h"

AGSPMasterGameMode::AGSPMasterGameMode()
{
	// set default pawn class to our Blueprinted character                     
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Prototypes/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
