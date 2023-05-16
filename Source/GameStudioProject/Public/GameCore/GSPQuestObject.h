// Game Studio Project Team F 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSPQuestObject.generated.h"


UCLASS(Blueprintable)
class UQuestObjectiveObjectBase : public UObject
{
	GENERATED_BODY()

public:

	//Called in BP to activate the objective object
	UFUNCTION(BlueprintImplementableEvent)
	void StartObjective();
	
};


/**
 * 
 */
UCLASS()
class GAMESTUDIOPROJECT_API UGSPQuestObject : public UPrimaryDataAsset
{

public:

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestName; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UQuestObjectiveObjectBase>> QuestObjectives;
	
};
