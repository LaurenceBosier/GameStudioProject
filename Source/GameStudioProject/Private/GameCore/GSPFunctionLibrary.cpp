// Game Studio Project Team F 2023


#include "GameCore/GSPFunctionLibrary.h"

#include "GameCore/GSPMasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

UActorComponent* UGSPFunctionLibrary::GetGSPComponentFromActor(AActor* InOtherActor,
	TSubclassOf<UGSPActorComponentBase> GetComponent, EValidCheck& OutResult)
{
	OutResult = EValidCheck::Failed;


	if(!IsValid(InOtherActor))
	{
		return nullptr;
	}

	UActorComponent* TempComponent = InOtherActor->GetComponentByClass(GetComponent);

	if(TempComponent)
	{
		OutResult = EValidCheck::Success;
		return TempComponent;
	}


	return nullptr;
}

UGSPMasterGameInstance* UGSPFunctionLibrary::GetGSPGameInstance(UObject* InWorldContextObject, EValidCheck& OutResult)
{
	OutResult = EValidCheck::Failed;

	//Check if world is valid
	if(InWorldContextObject && InWorldContextObject->GetWorld())
	{
		//Cast game instance to UGSPMasterGameInstance
		const auto TempGIRef = Cast<UGSPMasterGameInstance>(UGameplayStatics::GetGameInstance(InWorldContextObject->GetWorld()));

		//If cast result is valid return the new reference
		if(TempGIRef)
		{
			OutResult = EValidCheck::Success;
			return TempGIRef;
		}
		
	}

	//Failed to get master game instance 
	return nullptr;
}


UGSPMasterGameInstance* UGSPFunctionLibrary::GetGSPGameInstance(UObject* InWorldContextObject, bool InShouldLogError)
{
		//Check if world is valid
	if(InWorldContextObject && InWorldContextObject->GetWorld())
	{
		//Cast game instance to UGSPMasterGameInstance
		const auto TempGIRef = Cast<UGSPMasterGameInstance>(UGameplayStatics::GetGameInstance(InWorldContextObject->GetWorld()));

		//If cast result is valid return the new reference
		if(TempGIRef)
		{
			return TempGIRef;
		}
		
	}

	if(InShouldLogError)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GSPGameInstance!"));
	}

	//Failed to get master game instance 
	return nullptr;
}
