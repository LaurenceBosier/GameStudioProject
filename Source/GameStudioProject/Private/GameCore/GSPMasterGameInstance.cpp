// Game Studio Project Team F 2023 - Laurence Bosier

#include "GameCore/GSPMasterGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Gameplay/GSPInteractionComponent.h"
#include "Kismet/GameplayStatics.h"

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

void UGSPMasterGameInstance::Shutdown()
{
	//Stop tick timer before game instance is shutdown
	if(GetWorld())
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(InteractionTickHandle))
		{
			GetTimerManager().ClearTimer(InteractionTickHandle);
		}
	}
	Super::Shutdown();
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

bool UGSPMasterGameInstance::TryCreateGamePlayHUDWidget()
{
	//If the HUD class has been selected
	if(!GamePlayHUDClass)
	{
		return false;
	}
	
	//If there isn't an existing instance 
	if(!GamePlayHUDWidget)
	{
		GamePlayHUDWidget = CreateWidget<UUserWidget>(this, GamePlayHUDClass);

		if(GamePlayHUDWidget) 
		{
			GamePlayHUDWidget->AddToViewport(-1);
			return true;
		}
	}
	
	return false;
}

bool UGSPMasterGameInstance::TryInteractWithSelectedActor()
{
	//Return false if there are no components to interact with. 
	if(IntractableComponents.IsEmpty())
	{
		return false;
	}

	//Attempt to interact with each intractable component until one is valid or the end of the array  
	for (const auto& IntractableComponent : IntractableComponents)
	{
		//Attempt to interact with all valid intractable components
		if(IntractableComponent->IsPlayerObserving() && IntractableComponent->TryInteractWith())
		{
			//break out of the loop on the first valid interaction 
			return true;
		}
	}

	//None of the components in the array could be interacted with 
	return false;
}

void UGSPMasterGameInstance::AddOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent)
{
	IntractableComponents.AddUnique(InInteractionComponent);

	//Check if world is valid
	if(GetWorld())
	{
		//If the timer is not already running start the tick function at 30fps 
		if(!GetWorld()->GetTimerManager().IsTimerActive(InteractionTickHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(InteractionTickHandle, this, &UGSPMasterGameInstance::InteractionObservationTick, 0.33f, true);
		}
	}
}

void UGSPMasterGameInstance::RemoveOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent)
{
	IntractableComponents.Remove(InInteractionComponent);

	//Return if there are still components left
	if(!IntractableComponents.IsEmpty())
	{
		return;
	}

	//Check if world is valid
	if(GetWorld())
	{
		//If the timer is running, stop the tick timer
		if(GetWorld()->GetTimerManager().IsTimerActive(InteractionTickHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(InteractionTickHandle);
		}
	}

	//If no components are overlapped with remove the interaction pop-up 
	OnRemoveInteractionPopup();
}

void UGSPMasterGameInstance::InteractionObservationTick()
{
	//Return if there are no components to check 
	if(IntractableComponents.IsEmpty())
	{
		return;
	}

	//Loop through all overlapped components 
	for (const auto& IntractableComponent : IntractableComponents)
	{
		//If a components is being observed return
		if(IntractableComponent->IsPlayerObserving())
		{
			//Add interaction pop-up to player HUD
			OnAddInteractionPopup(IntractableComponent->InteractionMessage);
			return;
		}
	}

	//If no components where observed start the prompt to remove the interaction pop-up 
	OnRemoveInteractionPopup();
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
