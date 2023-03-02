// Game Studio Project Team F 2023


#include "Gameplay/GSPXpComponent.h"
#include "GameCore/GSPMasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UGSPXpComponent::AwardPlayerXP()
{
	//Return false if the component can no longer award XP 
	if(!bCanAwardXP)
	{
		return false;
	}

	if(!bUnlimitedUse)
	{
		if((componentUses + 1) > MaxUses)
		{
			bCanAwardXP = false;
			return false;
		}
	}

	if(!bReusable && componentUses > 0)
	{
		return false;
	}

	//If the world is valid 
	if(GetWorld())
	{
		//Cast to the GSP game instance and increment the player XP
		const auto GIRef = Cast<UGSPMasterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if(GIRef)
		{
			GIRef->AddPlayerXP(XpToAward, XpAwardReason);

			componentUses++;

			return true;
		}
	}

	UE_LOG(LogTemp,Error, TEXT("Something went wrong adding player xp"));
	return false;
}
