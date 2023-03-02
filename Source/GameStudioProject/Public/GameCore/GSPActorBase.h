// Game Studio Project Team F 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSPActorBase.generated.h"


UCLASS(HideCategories = ( 
	/*
	 Default Actor Class that must be used in this project!
	 Hides all default categories from the editor inspector
	 */
	"ActorTick", 
	"Replication",
	"Rendering",
	"Collision",
	"Actor",
	"Input",
	"HLOD",
	"Physics",
	"WorldPartition",
	"Events",
	"Cooking",
	"DataLayers"
	))

class GAMESTUDIOPROJECT_API AGSPActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGSPActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
