// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameCore/GSPActorComponentBase.h"
#include "GSPInteractionComponent.generated.h"

//Todo remove this as an 

class UGSPMasterGameInstance;

/* Indicates to the user interface what type of
 * interaction message should be displayed */
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
UCLASS(Blueprintable, ClassGroup=(GSP), meta=(BlueprintSpawnableComponent))
class GAMESTUDIOPROJECT_API UGSPInteractionComponent : public UGSPActorComponentBase
{
	GENERATED_BODY()

public:

	UGSPInteractionComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool TryInteractWith();

	/**
	 * @brief Checks if the player is looking at the owned actor 
	 * @return True if the player is looking at the owned actor 
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction Component")
	bool IsPlayerObserving() const;

protected:

	//Callback function for interaction radius OnBeginOverlap. 
	UFUNCTION()
	void OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Callback function for interaction radius OnBeginOverlap. 
	UFUNCTION()
	void OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RegisterCollisionComponent();

public:

#if WITH_EDITOR
	/**
	 * @brief Fired whenever a property is changed inside the editor defaults
	 * @param PropertyChangedEvent The event fired when a property is changed inside the component 
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	//True if the interaction radius should be shown in PIE (play-in-editor) 
	UPROPERTY(EditAnywhere, Category = "Interaction Component", AdvancedDisplay)
	bool bDebugShowInteractionRadius = false;

	//True if the component can be interacted with
	UPROPERTY(EditAnywhere, Category = "Interaction Component")
	bool bInteractable = true;

	//The message that will be displayed on the player HUD. 
	UPROPERTY(EditAnywhere, Category = "Interaction Component")
	EInteractionPopupMessage InteractionMessage = EInteractionPopupMessage::Interact;

	//True if the component can be interacted with an unlimited number of times
	UPROPERTY(EditAnywhere, Category = "Interaction Component")
	bool bUnlimitedUse = false;

	//The max amount of times the component can be interacted with 
	UPROPERTY(EditAnywhere, Category = "Interaction Component",meta = (EditCondition = "!bUnlimitedUse", EditConditionHides, ClampMin = "1", UIMin = "1"))
	int MaxInteractions = 1;

	//If the player needs to be looking in the general direction of the owning actor to be able to interact with it. 
	UPROPERTY(EditAnywhere, Category = "Interaction Component")
	bool bPlayerMustBeLooking = true;

	//How many degrees the player can look either direction of the actor before it becomes "out of view". 
	UPROPERTY(EditAnywhere, Category = "Interaction Component",meta = (EditCondition = "bPlayerMustBeLooking", EditConditionHides, ClampMin = "1", UIMin = "1", ClampMax = "180", UIMax = "180"))
	int LookAngleTolerance = 14;

	//The distance from the object the player can interact with the component
	UPROPERTY(EditAnywhere, Category = "Interaction Component", meta = (ClampMin = "20", UIMin = "20", ClampMax = "4000", UIMax = "4000"))
	int InteractionRadius = 250;


private:

	//Cached game instance ref
	UPROPERTY()
	UGSPMasterGameInstance* MasterGameInstanceRef { nullptr };

	//The collision sphere used to generate overlap events. 
	UPROPERTY()
	USphereComponent* InteractionRadiusCollider { nullptr };

	//Number of times the component has been interacted with 
	int InteractionCount = 0;

	/* Declare dynamic multi-cast delegates */

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInteractSignature,  UGSPInteractionComponent, OnInteract, int , OutRemainingInteractions);

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnLastInteractionSignature,  UGSPInteractionComponent, OnLastInteraction);

	/* End Declare dynamic multi-cast delegates */

public:

	/* Functions bound in blueprints */

	//OnInteractSignature Delegate, called when the component has been interacted with =. 
	UPROPERTY(BlueprintAssignable, Category = "Interaction Component")
	FOnInteractSignature OnInteract;

	//OnLastInteraction Delegate, called when the final interaction is called. 
	UPROPERTY(BlueprintAssignable, Category = "Interaction Component")
	FOnLastInteractionSignature OnLastInteraction;

	/* End Functions bound in blueprints */

};
