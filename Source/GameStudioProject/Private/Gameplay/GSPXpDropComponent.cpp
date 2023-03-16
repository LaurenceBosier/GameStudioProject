// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPXpDropComponent.h"

#include "GameCore/GSPFunctionLibrary.h"
#include "GameCore/GSPMasterGameInstance.h"

void UGSPXpDropComponent::BeginPlay()
{
	//Try set master game instance ref
	MasterGameInstanceRef = UGSPFunctionLibrary::GetGSPGameInstance(this);

	Super::BeginPlay();
}

bool UGSPXpDropComponent::GrantPlayerWithXP()
{
	//Return false if the component can no longer award XP 
	if(!bCanAwardXP)
	{
		return false;
	}

	//Increment component uses
	ComponentUses++;


	if(!bUnlimitedUse)
	{
		if(!bReusable || ComponentUses >= MaxUses)
		{
			bCanAwardXP = false;
		}
	}


	//If the world is valid 
	if(MasterGameInstanceRef && GetWorld())
	{
		MasterGameInstanceRef->AddPlayerXP(XpToAward, XpAwardReason);

		return true;
	}

	UE_LOG(LogTemp,Error, TEXT("Something went wrong adding player xp"));
	return false;
}
