// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameCore/GSPMasterGameInstance.h"
#include "GSPInteractionComponent.generated.h"


class UGSPMasterGameInstance;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(GSP), 
	meta=(BlueprintSpawnableComponent),
	HideCategories = ( 
	    /*
	    Default Actor Component Class that must be used in this project!
		Hides all default categories from the editor inspector,
		IMPORTANT: Starts with Tick() disabled by default
		*/
		"Variable", 
		"Transform", 
		"Shape", 
		"Navigation", 
		"HLOD", 
		"Rendering", 
		"Physics", 
		"Mobile", 
		"RayTracing", 
		"Sockets",
		"Tags",
		"ComponentTick",
		"ComponentReplication",
		"Activation",
		"Cooking",
		"Events",
		"AssetUserData",
		"Replication",
		"Collision"
		))
class GAMESTUDIOPROJECT_API UGSPInteractionComponent : public USphereComponent
{
	GENERATED_BODY()


public:

	UGSPInteractionComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Fired whenever a property is changed inside the editor defaults
	 * @param PropertyChangedEvent The event fired when a property is changed inside the component 
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	bool InteractWith();

protected:

	//Callback function for interaction radius OnBeginOverlap. 
	UFUNCTION()
	void OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Callback function for interaction radius OnBeginOverlap. 
	UFUNCTION()
	void OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Ticks every 16ms to see if the player is looking at the actor. 
	UFUNCTION()
	void TickLookAtRotation();

	FORCEINLINE bool IsPlayerLookingAtActor() const;

public:

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
	UPROPERTY(EditAnywhere, Category = "Interaction Component",meta = (EditCondition = "bPlayerMustBeLooking", EditConditionHides, ClampMin = "1", UIMin = "1", ClampMax = "360", UIMax = "360"))
	int LookAngleTolerance = 8;

	UPROPERTY(EditAnywhere, Category = "Interaction Component", meta = (ClampMin = "20", UIMin = "20", ClampMax = "4000", UIMax = "4000"))
	int InteractionRadius = 250;


private:

	UGSPMasterGameInstance* MasterGameInstance { nullptr };

	FTimerHandle LookatTimerHandle;

	class UCameraComponent* PlayerCameraComponentRef;

	bool bLookingAt = false;

	//If the interaction can take place
	bool bInteractionMessageShowing = false;

	int NumInteractions = 0;

	/* Declare dynamic multi-cast delegates */

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInteractSignature,  UGSPInteractionComponent, OnInteract, int , OutRemainingInteractions);

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnLastInteractionSignature,  UGSPInteractionComponent, OnLastInteraction);

	/* End Declare dynamic multi-cast delegates */

	/* Functions bound in blueprints */

	//OnInteractSignature Delegate, called when the component has been interacted with =. 
	UPROPERTY(BlueprintAssignable, Category = "Interaction Component")
	FOnInteractSignature OnInteract;

	//OnLastInteraction Delegate, called when the final interaction is called. 
	UPROPERTY(BlueprintAssignable, Category = "Interaction Component")
	FOnLastInteractionSignature OnLastInteraction;

	/* End Functions bound in blueprints */

};
