// Game Studio Project Team F 2023


#include "GameCore/GSPActorBase.h"

// Sets default values
AGSPActorBase::AGSPActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGSPActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGSPActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

