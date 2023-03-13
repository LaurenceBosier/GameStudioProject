// Game Studio Project Team F 2023 - Laurence Bosier

#include "GameCore/GSPMasterGameInstance.h"

#include "Gameplay/GSPInteractionComponent.h"

void UGSPMasterGameInstance::Init()
{
	Super::Init();

	//Throw error if no level up curve is defined 
	if(XPLevelUpCurve == nullptr)
	{
		UE_LOG(LogBlueprint, Error, TEXT("Game instance is missing required valid XP curve"));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Game instance is missing required valid XP curve"), true, FVector2D(1.5f));
		}
		return;
	}

	//read the min and max allowed player levels from the curve asset 
	XPLevelUpCurve->GetTimeRange(MinPlayerLevel, MaxPlayerLevel);

	if(MinPlayerLevel != 0)
	{
		UE_LOG(LogBlueprint, Error, TEXT("Game instance XP Curve must start from a time of zero"));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Game instance XP Curve must start from a time of zero"), true, FVector2D(1.5f));
		}
		return;
	}

	if (MaxPlayerLevel <= MinPlayerLevel)
	{
		UE_LOG(LogBlueprint, Error, TEXT("Game instance XP curve has invalid max level"));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Game instance XP curve has invalid max level"), true, FVector2D(1.5f));
		}
		return;
	}

	//Find the amount of XP required to level up
	RequiredXpForLevelUp = static_cast<int>(XPLevelUpCurve->GetFloatValue(CurrentPlayerLevel));
}

void UGSPMasterGameInstance::AddPlayerXP(int InXpAmount, EXpAwardType InUserInterfacePrompt)
{
	if((CurrentPlayerXP + InXpAmount) >= RequiredXpForLevelUp)
	{
		LevelUp((CurrentPlayerXP + InXpAmount) - RequiredXpForLevelUp);
		return;
	}

	CurrentPlayerXP += InXpAmount;
}

bool UGSPMasterGameInstance::TryInteractWithSelectedActor(APawn* Self)
{
	//Return false if there are no components to interact with. 
	if(IntractableComponents.IsEmpty())
	{
		UE_LOG(LogTemp,Warning, TEXT("No overlapped components"));
		return false;
	}

	//Attempt to interact with each intractable component until one is valid or the end of the array  
	for (const auto& IntractableComponent : IntractableComponents)
	{
		//Attempt to interact with all valid intractable components
		if(IntractableComponent->IsPlayerObserving(Self) && IntractableComponent->TryInteractWith())
		{
			//break out of the loop on the first valid interaction 
			return true;
		}
	}

	UE_LOG(LogTemp,Warning, TEXT("No interactable components"));

	//None of the components in the array could be interacted with 
	return false;
}

void UGSPMasterGameInstance::AddInteractionPopup(EInteractionPopupMessage InInteractionType)
{
	//Broadcast OnAddInteractionPopup to blueprint 
	OnAddInteractionPopup(InInteractionType);
}

void UGSPMasterGameInstance::AddOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent)
{
	IntractableComponents.AddUnique(InInteractionComponent);
}

void UGSPMasterGameInstance::RemoveOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent)
{
	IntractableComponents.Remove(InInteractionComponent);
}


bool UGSPMasterGameInstance::LevelUp(int InOverflowXp, EXpAwardType InUserInterfacePrompt)
{

	//Set current player XP to zero
	CurrentPlayerXP = 0;

	//Return if the player is at max level
	if(CurrentPlayerLevel >= MaxPlayerLevel)
	{
		return true;
	}


	//Find the next amount of required XP 
	RequiredXpForLevelUp = static_cast<int>(XPLevelUpCurve->GetFloatValue(CurrentPlayerLevel));

	//Increment the players level 
	CurrentPlayerLevel++;

	//If there is leftover XP after leveling up, add it to the next level 
	if(InOverflowXp > 0)
	{
		AddPlayerXP(InOverflowXp, InUserInterfacePrompt);
	}

	return false;

}
