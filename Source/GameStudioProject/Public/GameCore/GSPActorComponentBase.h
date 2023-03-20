// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GSPActorComponentBase.generated.h"


UCLASS(Blueprintable, ClassGroup=(GSP), 
	HideCategories = ( 
	    /*
	    Default Actor Component Class that must be used in this project!
		Hides all default categories from the editor inspector,
		IMPORTANT: Starts with Tick() disabled by default
		*/
		"Variable", 
		"Sockets",
		"Tags",
		"HLOD",
		"Navigation",
		"Rendering",
		"Physics",
		"ComponentTick",
		"ComponentReplication",
		"Activation",
		"Cooking",
		"Events",
		"AssetUserData",
		"Replication",
		"Collision",
		"Transform", 
		"Shape", 
		"Mobile", 
		"RayTracing",
		"LOD",
		"StaticMesh",
		"TextureStreaming",
		"MaterialParameters",
		"VirtualTexture",
		"Lighting"
		))
class GAMESTUDIOPROJECT_API UGSPActorComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGSPActorComponentBase();

};
