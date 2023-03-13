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
enum class EInteractionPopupMessage : uint8
{
	Interact UMETA(ToolTip="Displays 'Interact'on the players HUD, a general interaction message"), 
	Open UMETA(ToolTip="Displays 'Open'on the players HUD, useful for opening chests"),
	PickUp UMETA(ToolTip="Displays 'Pick Up'on the players HUD, useful for adding physical items to the players inventory"),
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

	/**
	 * @brief Initializes the Xp system
	 */
	virtual void Init() override;

	/**
	 * @brief Adds xp to the player and levels up when necessary
	 * @param InXpAmount The amount of xp the player was awarded
	 * @param InUserInterfacePrompt The reason for adding Xp
	*/
	UFUNCTION(Category = "XP")
	void AddPlayerXP(int InXpAmount, EXpAwardType InUserInterfacePrompt);

	/**
	 * @brief Attempts to interact with the selected actor. Safe to call whenever
	 * @return True if the interaction was successful
	 */
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf = InSelf, HideSelfPin))
	bool TryInteractWithSelectedActor(APawn* InSelf);

	/**
	 * @brief Increments NumIntractableActors and displays the most recent EInteractionTypeMessage
	 * @param InInteractionType Specifies what sort of interaction text will be displayed on the player HUD
	 */
	UFUNCTION()
	void AddInteractionPopup(EInteractionPopupMessage InInteractionType);

	/**
	 * @brief Adds a c
	 * @param InInteractionComponent 
	 */
	UFUNCTION()
	void AddOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent);

	UFUNCTION()
	void RemoveOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent);

	/**
	 * @brief Called when the interaction popup should be displayed on the players HUD
	 * @param OutPopupMessage The type of interaction for the popup message
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnAddInteractionPopup(EInteractionPopupMessage OutPopupMessage);

	/**
	 * @brief Called when the interaction popup should be removed from the player HUD
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnRemoveInteractionPopup();


	/**
	 * @brief Getter for the players current level xp 
	 * @return The CurrentPlayerXP
	 */
	UFUNCTION(BlueprintCallable, Category = "XP")
	FORCEINLINE int GetCurrentPlayerLevelXp() const { return CurrentPlayerXP; }

	/**
	 * @brief Getter for the players current level 
	 * @return The CurrentPlayerLevel
	 */
	UFUNCTION(BlueprintCallable, Category = "XP")
	FORCEINLINE int GetCurrentPlayerLevel() const { return CurrentPlayerLevel; }

	/**
	 * @brief Getter for the required xp to level up  
	 * @return The RequiredLevelUpXP 
	 */
	UFUNCTION(BlueprintCallable, Category = "XP")
	FORCEINLINE int GetRequiredXpForLevelUp() const { return RequiredXpForLevelUp; }

	

private:

	/**
	 * @brief Attempts to level the player up
	 * @param InOverflowXp The amount of Xp gained, over the required amount of the level up
	 * @param InUserInterfacePrompt The reason for adding Xp
	 * @return True if the player leveled up
	 */
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
	UPROPERTY() //UPROPERTY() insures that the pointer is set to null if there are no references left to the actor component.
	TArray<UGSPInteractionComponent*> IntractableComponents;

	//The amount of XP the player has in the current level
	UPROPERTY()
	int CurrentPlayerXP;

	//The amount of XP the player requires to level up 
	UPROPERTY()
	int RequiredXpForLevelUp;

	//Used to validate the data supplied by the XP curve
	float MaxPlayerLevel;
	float MinPlayerLevel;

};
