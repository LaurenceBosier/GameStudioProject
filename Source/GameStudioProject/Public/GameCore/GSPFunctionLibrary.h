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
