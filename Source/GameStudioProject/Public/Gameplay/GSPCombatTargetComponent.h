// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "GameCore/GSPActorComponentBase.h"
#include "GSPCombatTargetComponent.generated.h"

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
		"AssetUserData",
		"Replication",
		"Collision",
		"LOD",
		"StaticMesh",
		"TextureStreaming",
		"MaterialParameters",
		"VirtualTexture",
		"Lighting"
		))
class GAMESTUDIOPROJECT_API UGSPCombatTargetComponent : public UGSPActorComponentBase
{
	GENERATED_BODY()

public:

	UGSPCombatTargetComponent();

	virtual void BeginPlay() override;

	//Attempts to populate the StaticMeshBones array with bones from the owners skeletal mesh
	UFUNCTION(CallInEditor, Category = "Combat Target Component")
	void AttemptToPopulateStaticMeshBones();

public:

	//If the component should attempt to attach the target marker 
	UPROPERTY(EditAnywhere, Category = "Combat Target Component")
	bool bAutoAttachToSkeletalMesh = true;

	//Used to attach the target marker to the skeletal mesh
	UPROPERTY(EditAnywhere, Category = "Combat Target Component", 
		meta = (ToolTip="The bone name or socket name that can be locked onto by the player, paste from StaticMeshBones list",
			EditCondition = "bAutoAttachToSkeletalMesh", EditConditionHides))
	FName LockOnMarkerSocketOrBoneName;

	//Used to show the user the bones and sockets from the owners skeletal mesh
	UPROPERTY(VisibleInstanceOnly, Category = "Combat Target Component", meta = (EditCondition = "bAutoAttachToSkeletalMesh", EditConditionHides))
	TArray<FName> StaticMeshBones;

	//Used to place a marker somewhere on a static mesh
	UPROPERTY(EditAnywhere, Category = "Combat Target Component", meta = (EditCondition = "!bAutoAttachToSkeletalMesh", EditConditionHides))
	FVector TargetMarkerTransformOffset;


private:

	UPROPERTY()
	USkeletalMeshComponent* FoundSkeletalMesh;
};
