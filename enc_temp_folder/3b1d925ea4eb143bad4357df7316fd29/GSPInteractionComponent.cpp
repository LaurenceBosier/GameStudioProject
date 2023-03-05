// Game Studio Project Team F 2023


#include "Gameplay/GSPInteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraModifier.h"
#include "GameCore/GSPMasterGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UGSPInteractionComponent::UGSPInteractionComponent()
{
	InitSphereRadius(InteractionRadius);
	SetVisibility(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ShapeColor = FColor::Emerald;
	bHiddenInGame = true;

	SetComponentTickEnabled(false);
}

void UGSPInteractionComponent::BeginPlay()
{
	OnComponentBeginOverlap.AddDynamic(this, &UGSPInteractionComponent::OnBeginInteractionOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UGSPInteractionComponent::OnEndInteractionOverlap);

	if(GetWorld())
	{
		MasterGameInstance = Cast<UGSPMasterGameInstance>( UGameplayStatics::GetGameInstance(GetWorld()));
	}

	if(!MasterGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction component faild to find Master Game Insatnce!"));
	}

	Super::BeginPlay();
}

void UGSPInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(LookatTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LookatTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}


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

bool UGSPInteractionComponent::InteractWith()
{
	if(bInteractable)
	{
		//increment interaction count
		NumInteractions ++;
		
		//Broadcast interaction event to blueprint class
		UGSPInteractionComponent::OnInteract.Broadcast<int>(MaxInteractions - NumInteractions);

		if(!bUnlimitedUse && NumInteractions >= MaxInteractions)
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

void UGSPInteractionComponent::OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!MasterGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction Component: No valid master game insatnce!"));
		return;
	}

	//If the player doesn't need to be looking, add the interaction message and set the intractable component. 
	if(!bPlayerMustBeLooking && !bInteractionMessageShowing)
	{
		bInteractionMessageShowing = true;
		MasterGameInstance->AddInteractionPopup(InteractionMessage);
		MasterGameInstance->SetSelectedInteractionComponent(this);
		return;
	}

	//Start checking if the player is looking in the direction of the owning actor. 
	if(GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(LookatTimerHandle))
	{
		//Get player camera ref 
		PlayerCameraComponentRef = Cast<UCameraComponent>(OtherActor->GetComponentByClass(UCameraComponent::StaticClass()));

		//If the camera ref is valid start the timer 
		if(PlayerCameraComponentRef)
		{
			GetWorld()->GetTimerManager().SetTimer(LookatTimerHandle, this, &UGSPInteractionComponent::TickLookAtRotation ,0.016f,true, 0);
		}
	}
}

void UGSPInteractionComponent::OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!MasterGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction Component: No valid master game insatnce!"));
		return;
	}

	//removed interaction message
	if(bInteractionMessageShowing)
	{
		MasterGameInstance->RemoveInteractionMessage();
	}


	//If the look-at timer is active then cancel it
	if(GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(LookatTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LookatTimerHandle);
	}
	
}


void UGSPInteractionComponent::TickLookAtRotation()
{

	if(!bInteractable)
	{
		return;
	}

	//bLookingAt is used as an optimization to not always call IsPlayerLookingAtActor();

	if(PlayerCameraComponentRef && MasterGameInstance)
	{
		if(!bInteractionMessageShowing && !bLookingAt && IsPlayerLookingAtActor())
		{
			bLookingAt = true;
			bInteractionMessageShowing = true;
			MasterGameInstance->AddInteractionPopup(InteractionMessage);
			MasterGameInstance->SetSelectedInteractionComponent(this);
		}
		else if(bInteractionMessageShowing && bLookingAt && !IsPlayerLookingAtActor())
		{
			bLookingAt = false;
			bInteractionMessageShowing = false;
			MasterGameInstance->RemoveInteractionMessage();
			MasterGameInstance->ClearSelectedInteractionComponent();
		}
	}

}

bool UGSPInteractionComponent::IsPlayerLookingAtActor() const
{
	if(PlayerCameraComponentRef && GetOwner())
	{
		const float a = FMath::Abs(PlayerCameraComponentRef->GetComponentRotation().Yaw);

		const float b = FMath::Abs(UKismetMathLibrary::FindLookAtRotation(PlayerCameraComponentRef->GetComponentLocation(), GetOwner()->GetActorLocation()).Yaw);

		return UKismetMathLibrary::InRange_FloatFloat(a, b - LookAngleTolerance, b + LookAngleTolerance);
	}
	return false;
}
