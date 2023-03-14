// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "GameCore/GSPActorComponentBase.h"
#include "GSPXpDropComponent.generated.h"

class UGSPMasterGameInstance;
/* Indicates to the user interface what type of
 * interaction causes the xp to be granted */
UENUM(BlueprintType)
enum class EXpAwardType : uint8
{
	None UMETA(DisplayName = "Hide on HUD", ToolTip="When XP is granted it will not be added to the user HUD"), 
	Interaction UMETA(DisplayName = "Interaction", ToolTip="Awarded xp will be added to HUD with the message 'Interaction +(num)xp'"),
	Kill UMETA(DisplayName = "Enemy Kill", ToolTip="Awarded xp will be added to HUD with the message 'Enemy Kill +(num)xp'"),
	QuestCompletion UMETA(DisplayName = "Quest Completion", ToolTip="Awarded xp will be added to HUD with the message 'Quest Complete +(num)xp'") 
};

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(GSP), 
	meta=(BlueprintSpawnableComponent))
class GAMESTUDIOPROJECT_API UGSPXpDropComponent : public UGSPActorComponentBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	/**
	 * @brief Grants the player with the amount of XP points specified by the component instance
	 * @return True if the xp was awarded to the player
	 */
	UFUNCTION(BlueprintCallable, Category = "Xp Component")
	bool GrantPlayerWithXP();

public:

	//The reason for awarding the player with XP. 
	UPROPERTY(EditAnywhere, Category = "Xp Component")
	EXpAwardType XpAwardReason;

	//The amount of Xp the component should grant the player
	UPROPERTY(EditAnywhere, Category = "Xp Component", meta = (ClampMin = "1", UIMin = "1"))
	int XpToAward;

	//If the component can award xp more than once.
	UPROPERTY(EditAnywhere, Category = "Xp Component")
	bool bReusable = false;

	//If the component can award xp an unlimited number of times.
	UPROPERTY(EditAnywhere, Category = "Xp Component", meta = (EditCondition = "bReusable", EditConditionHides))
	bool bUnlimitedUse = false;

	//The number of times the component can award xp for an action. (0 = unlimited)
	UPROPERTY(EditAnywhere, Category = "Xp Component", meta = (EditCondition = "bReusable && !bUnlimitedUse", EditConditionHides, ClampMin = "1", UIMin = "1"))
	int MaxUses = 1;


private:

	//Reference to the game instance
	UGSPMasterGameInstance* MasterGameInstanceRef { nullptr };

	//The number of times the component has granted the player with XP. 
	int ComponentUses = 0;

	//If the component is able to award the player with XP. 
	bool bCanAwardXP = true; 
};
