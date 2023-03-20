// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPInteractionComponent.h"

#include "GameCore/GSPMasterGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <GameCore/GSPFunctionLibrary.h>

//Todo filter interaction overlap to player only (not just filtering by Pawn type)

UGSPInteractionComponent::UGSPInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UGSPInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Try set master game instance ref
	MasterGameInstanceRef = UGSPFunctionLibrary::GetGSPGameInstance(this);

	//Create interaction overlap collider
	RegisterCollisionComponent();

#if WITH_EDITOR

	InteractionRadiusCollider->SetHiddenInGame(!bDebugShowInteractionRadius);

#endif

}


void UGSPInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InteractionRadiusCollider->UnregisterComponent();

	Super::EndPlay(EndPlayReason);
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

bool UGSPInteractionComponent::IsPlayerObserving() const //Todo fix this
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

	FVector EyeLocation; 
	FRotator EyeRotation;

	InPlayerCharacter->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector NormalizedLoc = (InteractionRadiusCollider->GetComponentToWorld().GetLocation() - EyeLocation);

	NormalizedLoc.Normalize();

	const float Angle = FMath::Abs(FVector::DotProduct(NormalizedLoc, UKismetMathLibrary::GetForwardVector(InPlayerCharacter->GetControlRotation())));

	return (1 - Angle) < static_cast<float>(LookAngleTolerance) / 100;
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

void UGSPInteractionComponent::RegisterCollisionComponent()
{
	InteractionRadiusCollider = NewObject<USphereComponent>(GetOwner(), TEXT("Interaction radius v2"));

	InteractionRadiusCollider->SetupAttachment(GetOwner()->GetRootComponent());

	InteractionRadiusCollider->CreationMethod = EComponentCreationMethod::Native;

	InteractionRadiusCollider->ShapeColor = FColor::Emerald;

	InteractionRadiusCollider->bHiddenInGame = false;

	InteractionRadiusCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionRadiusCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//Bind callback events for collision 
	InteractionRadiusCollider->OnComponentBeginOverlap.AddDynamic(this, &UGSPInteractionComponent::OnBeginInteractionOverlap);
	InteractionRadiusCollider->OnComponentEndOverlap.AddDynamic(this, &UGSPInteractionComponent::OnEndInteractionOverlap);

	//Find the center of the owned actor (based on collision components) and set the interaction collision to that location
	if(GetOwner() && GetOwner()->GetRootComponent())
	{
		InteractionRadiusCollider->InitSphereRadius(InteractionRadius);

		InteractionRadiusCollider->SetupAttachment(GetOwner()->GetRootComponent());

		FVector ActorCenter, UselessExtent;

		GetOwner()->GetActorBounds(true, ActorCenter, UselessExtent);

		//World location to Relative location 
		ActorCenter = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetActorTransform(), ActorCenter);

		//Set the relative location of the collision sphere to the center of owner actor
		InteractionRadiusCollider->SetRelativeLocation(ActorCenter);
	}

	//Register the component in the game
	InteractionRadiusCollider->RegisterComponent();
}

#if WITH_EDITOR

void UGSPInteractionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if(PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGSPInteractionComponent, InteractionRadius))
		{
			if(GetOwner() && GetOwner()->GetRootComponent())
			{
				FVector ActorCenter, UselessExtent;

				GetOwner()->GetActorBounds(true, ActorCenter, UselessExtent);

				if(GetWorld())
				{
					DrawDebugSphere(GetWorld(), ActorCenter, InteractionRadius, 12, FColor::Cyan, false, 0.2f, 0, 2);
				}
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif