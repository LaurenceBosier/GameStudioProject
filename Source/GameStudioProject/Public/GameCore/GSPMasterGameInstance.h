// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gameplay/GSPXpDropComponent.h"
#include "GSPMasterGameInstance.generated.h"

class UGSPInteractionComponent;
/* Indicates to the user interface what type of
 * interaction causes the xp to be granted */
UENUM(BlueprintType)
enum class EInteractionTypeMessage : uint8
{
	Interact UMETA(ToolTip="Displays 'Interact'on the players HUD, a general interaction message"), 
	Open UMETA(ToolTip="Displays 'Open'on the players HUD, useful for opening chests"),
	Push UMETA(ToolTip="Displays 'Push'on the players HUD, useful for pushable objects"),
	Talk UMETA(ToolTip="Displays 'Talk'on the players HUD, useful for NPC interactions") 
};


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

	/**
	 * @brief Attempts to interact with the selected actor. Safe to call whenever
	 * @return True if the interaction was successful
	 */
	UFUNCTION(BlueprintCallable)
	bool InteractWithSelectedActor();

	/**
	 * @brief Increments NumIntractableActors and displays the most recent EInteractionTypeMessage
	 * @param InInteractionType Specifies what sort of interaction text will be displayed on the player HUD
	 */
	UFUNCTION()
	void AddInteractionMessage(EInteractionTypeMessage InInteractionType);

	/**
	 * @brief Sets the selected interaction component
	 * @param InInteractionComponent 
	 */
	UFUNCTION()
	void SetSelectedInteractionComponent(UGSPInteractionComponent* InInteractionComponent);

	/**
	 * @brief Clears the selected interaction component
	 * */
	UFUNCTION()
	void ClearSelectedInteractionComponent();

	/**
	 * @brief Decrements NumIntractableActors, will remove message from HUD when count falls below one
	 */
	UFUNCTION()
	void RemoveInteractionMessage();


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

	//The currently selected interaction component
	UPROPERTY()
	UGSPInteractionComponent* SelectedInteractionComponent { nullptr };

	//The number of intractable actors the player is overlapping. 
	UPROPERTY()
	int NumIntractableActors { 0 };

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
