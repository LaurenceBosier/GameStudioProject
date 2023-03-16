// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gameplay/GSPXpDropComponent.h"
#include "GSPMasterGameInstance.generated.h"

class UGSPInteractionComponent;
enum class EInteractionPopupMessage : uint8;

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
	 * @brief Ends the interaction tick timer if valid
	 */
	virtual void Shutdown() override;

	/**
	 * @brief Adds xp to the player and levels up when necessary
	 * @param InXpAmount The amount of xp the player was awarded
	 * @param InUserInterfacePrompt The reason for adding Xp
	*/
	UFUNCTION(Category = "XP")
	void AddPlayerXP(int InXpAmount, EXpAwardType InUserInterfacePrompt);

	/**
	 * @brief Attempts to create and add the game play HUD to the screen 
	 * @return True if the widget was successfully created and added to the screen, false if failed or instance already exists 
	 */
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	bool TryCreateGamePlayHUDWidget();

	/**
	 * @brief Attempts to interact with the selected actor. Safe to call whenever
	 * @return True if the interaction was successful
	 */
	UFUNCTION(BlueprintCallable)
	bool TryInteractWithSelectedActor();

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
	 * @brief Adds the component to the overlapped array
	 * @param InInteractionComponent The overlapped interaction component
	 */
	UFUNCTION()
	void AddOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent);

	/**
	 * @brief Removes the component to the overlapped array
	 * @param InInteractionComponent The overlapped interaction component
	 */
	UFUNCTION()
	void RemoveOverlappedInteractionComponent(UGSPInteractionComponent* InInteractionComponent);


private:

	/**
	 * @brief Ticks at 30fps to see if the player is looking at any overlapped intractable components 
	 */
	UFUNCTION()
	void InteractionObservationTick();

	/**
	 * @brief Getter for the players current level xp 
	 * @return The CurrentPlayerXP
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "XP")
	FORCEINLINE int GetCurrentPlayerLevelXp() const { return CurrentPlayerXP; }

	/**
	 * @brief Getter for the players current level 
	 * @return The CurrentPlayerLevel
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "XP")
	FORCEINLINE int GetCurrentPlayerLevel() const { return CurrentPlayerLevel; }

	/**
	 * @brief Getter for the required xp to level up  
	 * @return The RequiredLevelUpXP 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "XP")
	FORCEINLINE int GetRequiredXpForLevelUp() const { return RequiredXpForLevelUp; }

	/**
	 * @brief Getter for the players game play widget instance 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Interface")
	FORCEINLINE class UUserWidget* GetGamePlayHUDWidget() { return GamePlayHUDWidget; }

	/**
	 * @brief Attempts to level the player up
	 * @param InOverflowXp The amount of Xp gained, over the required amount of the level up
	 * @param InUserInterfacePrompt The reason for adding Xp
	 * @return True if the player leveled up
	 */
	bool LevelUp(int InOverflowXp = 0, EXpAwardType InUserInterfacePrompt= EXpAwardType::None);


public:

	//The widget class that will be displayed as the players main HUD
	UPROPERTY(EditAnywhere, Category = "User Interface")
	TSubclassOf<class UUserWidget> GamePlayHUDClass;

	//The instance of the created widget
	class UUserWidget* GamePlayHUDWidget { nullptr };

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

	//Timer used to tick if the player is looking at any intractable components 
	FTimerHandle InteractionTickHandle;

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
