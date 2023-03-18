// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSPFunctionLibrary.generated.h"


UENUM(BlueprintType)
enum class EValidCheck : uint8
{
	Success, //The function was executed successfully 
	Failed   //The function encountered an error  
};

/**
 * 
 */
UCLASS()
class GAMESTUDIOPROJECT_API UGSPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Try's to get the GSP actor component from the inputted actor
	 * @param InOtherActor The actor you want to try and get the component from
	 * @param GetComponent The component type you are trying to get from the actor
	 * @param OutResult If the component was gotten successfully
	 * @return The found instance of the component, or nullptr if no component was found on the actor 
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Studio Project Utility", meta =(ExpandEnumAsExecs = "OutResult", DeterminesOutputType = "GetComponent"))
	static class UActorComponent* GetGSPComponentFromActor(AActor* InOtherActor, class TSubclassOf<class UGSPActorComponentBase> GetComponent, EValidCheck& OutResult);

	/**
	 * @brief Attempts to get the UGSPMasterGameInstance 
	 * @param InWorldContextObject Caller 
	 * @param OutResult The success state of the function
	 * @return A pointer to the UGSPMasterGameInstance instance, or nullptr on fail
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Studio Project Utility", meta =(ExpandEnumAsExecs = "OutResult", WorldContext="InWorldContextObject"))
	static class UGSPMasterGameInstance* GetGSPGameInstance(UObject* InWorldContextObject, EValidCheck& OutResult);

	/**
	 * @brief Attempts to get the UGSPMasterGameInstance 
	 * @param InWorldContextObject Caller
	 * @param InShouldLogError True if the function should log an error on fail
	 * @return A pointer to the UGSPMasterGameInstance instance, or nullptr on fail
	 */
	static class UGSPMasterGameInstance* GetGSPGameInstance(UObject* InWorldContextObject, bool InShouldLogError = true);
};
