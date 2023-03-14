// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPXpDropComponent.h"
#include "GameCore/GSPMasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UGSPXpDropComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetWorld())
	{
		//Cast to the GSP game instance and increment the player XP
		MasterGameInstanceRef = Cast<UGSPMasterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Xp Drop component Faild to get game isnatnce ref"));
	}
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
