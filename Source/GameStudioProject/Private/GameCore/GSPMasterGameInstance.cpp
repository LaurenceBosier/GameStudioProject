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
	RequiredLevelUpXP = static_cast<int>(XPLevelUpCurve->GetFloatValue(CurrentPlayerLevel));

}

void UGSPMasterGameInstance::AddPlayerXP(int InXpAmount, EXpAwardType InUserInterfacePrompt)
{
	if((CurrentPlayerXP + InXpAmount) >= RequiredLevelUpXP)
	{
		LevelUp((CurrentPlayerXP + InXpAmount) - RequiredLevelUpXP);
		return;
	}

	CurrentPlayerXP += InXpAmount;
}

bool UGSPMasterGameInstance::InteractWithSelectedActor()
{
	if(SelectedInteractionComponent)
	{
		return SelectedInteractionComponent->InteractWith();
	}
	return false;
}

void UGSPMasterGameInstance::AddInteractionMessage(EInteractionTypeMessage InInteractionType)
{
	NumIntractableActors++;
}

void UGSPMasterGameInstance::RemoveInteractionMessage()
{
	NumIntractableActors--;

	if(NumIntractableActors <= 0)
	{
		NumIntractableActors = 0;

		//Todo start countdown to remove HUD
	}
}

void UGSPMasterGameInstance::SetSelectedInteractionComponent(UGSPInteractionComponent* InInteractionComponent)
{
	SelectedInteractionComponent = InInteractionComponent;
}

void UGSPMasterGameInstance::ClearSelectedInteractionComponent()
{
	SelectedInteractionComponent = nullptr;
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
	RequiredLevelUpXP = static_cast<int>(XPLevelUpCurve->GetFloatValue(CurrentPlayerLevel));

	//Increment the players level 
	CurrentPlayerLevel++;

	//If there is leftover XP after leveling up, add it to the next level 
	if(InOverflowXp > 0)
	{
		AddPlayerXP(InOverflowXp, InUserInterfacePrompt);
	}

	return false;

}
