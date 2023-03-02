// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gameplay/GSPXpComponent.h"
#include "GSPMasterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMESTUDIOPROJECT_API UGSPMasterGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	void Init() override;

	/**
	 * @brief Adds xp to the player and levels up when necessary
	 * @param InXpAmount The amount of xp the player was awarded
	 * @param InUserInterfacePrompt Responsible for setting the text on the user interface
	 */
	UFUNCTION(Category = "XP")
	void AddPlayerXP(int InXpAmount, EXpAwardType InUserInterfacePrompt);


	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCurrentPlayerLevelXp() const { return CurrentPlayerXP; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCurrentPlayerLevel() const { return CurrentPlayerLevel; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetLevelRequiredXp() const { return RequiredLevelUpXP; }

	

private:

	bool LevelUp(int InOverflowXp = 0, EXpAwardType InUserInterfacePrompt= EXpAwardType::None);

public:

	//The players current level
	UPROPERTY(EditDefaultsOnly, Category = "XP")
	int CurrentPlayerLevel = 0;

	//A curve used to determine how much XP is required to level up, per level.
	//Each time step on the horizontal axis represents 1 level.
	//The vertical axis represents the amount of XP required to level up. 
	UPROPERTY(EditDefaultsOnly, Category = "XP", meta = (DisplayName = "XP Level Up Curve"))
	UCurveFloat* XPLevelUpCurve; 

private:

	//The amount of XP the player has in the current level
	UPROPERTY()
	int CurrentPlayerXP;

	//The amount of XP the player requires to level up 
	UPROPERTY()
	int RequiredLevelUpXP;

	//Used to validate the data supplied by the XP curve
	float MaxPlayerLevel;
	float MinPlayerLevel; 
};
