// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPInteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraModifier.h"
#include "GameCore/GSPMasterGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <GameCore/GSPFunctionLibrary.h>

//Todo filter interaction overlap to player only (not just filtering by Pawn type)

UGSPInteractionComponent::UGSPInteractionComponent()
{
	InitSphereRadius(InteractionRadius);
	SetVisibility(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ShapeColor = FColor::Emerald;
	
	bHiddenInGame = true;
	PrimaryComponentTick.bCanEverTick = false;
}

void UGSPInteractionComponent::BeginPlay()
{
	//Bind callback events for collision 
	OnComponentBeginOverlap.AddDynamic(this, &UGSPInteractionComponent::OnBeginInteractionOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UGSPInteractionComponent::OnEndInteractionOverlap);

	//Try set master game instance ref
	MasterGameInstanceRef = UGSPFunctionLibrary::GetGSPGameInstance(this);

	Super::BeginPlay();

#if WITH_EDITOR

	SetHiddenInGame(!bDebugShowInteractionRadius);

#endif

}

bool UGSPInteractionComponent::TryInteractWith()
{
	if(bInteractable)
	{
		//Increment interaction count
		InteractionCount ++;
		
		//Broadcast interaction event to blueprint class
		UGSPInteractionComponent::OnInteract.Broadcast<int>(MaxInteractions - InteractionCount);

		if(!bUnlimitedUse && InteractionCount >= MaxInteractions)
		{
			//Update intractable state if max uses has been exceeded 
			bInteractable = false;

			//Broadcast last interaction event to blueprint class
			UGSPInteractionComponent::OnLastInteraction.Broadcast();
		}

		return true;
	}

	return false;
}

bool UGSPInteractionComponent::IsPlayerObserving() const
{
	if(GetWorld() == nullptr)
	{
		return false;
	}

	const auto* InPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(InPlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No char!"));
		return false;
	}


	FVector NormalizedLoc =  (this->GetOwner()->GetActorLocation() - InPlayerCharacter->GetActorLocation());
	NormalizedLoc.Normalize();

	const float Angle = FVector::DotProduct(NormalizedLoc, UKismetMathLibrary::GetForwardVector(InPlayerCharacter->GetControlRotation()));

	constexpr float DirectViewTolerance = 1;

	return FMath::Abs((DirectViewTolerance - Angle)) < static_cast<float>(LookAngleTolerance) / 100;
}

void UGSPInteractionComponent::OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!MasterGameInstanceRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction Component: No valid master game insatnce!"));
		return;
	}

	MasterGameInstanceRef->AddOverlappedInteractionComponent(this);
}

void UGSPInteractionComponent::OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!MasterGameInstanceRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction Component: No valid master game insatnce!"));
		return;
	}

	MasterGameInstanceRef->RemoveOverlappedInteractionComponent(this);
}

#if WITH_EDITOR

void UGSPInteractionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if(PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGSPInteractionComponent, InteractionRadius))
		{
			SetSphereRadius(InteractionRadius);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif