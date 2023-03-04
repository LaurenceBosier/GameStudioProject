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
	if(GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(LookatTimerHandle))
	{
		PlayerCameraComponentRef = Cast<UCameraComponent>(OtherActor->GetComponentByClass(UCameraComponent::StaticClass()));
		if(PlayerCameraComponentRef)
		{
			GetWorld()->GetTimerManager().SetTimer(LookatTimerHandle, this, &UGSPInteractionComponent::TickLookatRotation ,0.04f,true, 0);
		}
	}
}

void UGSPInteractionComponent::OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(LookatTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LookatTimerHandle);
	}

	if(MasterGameInstance && bLookingAt)
	{
		MasterGameInstance->RemoveInteractionMessage();
	}

	bLookingAt = false;
}


void UGSPInteractionComponent::TickLookatRotation()
{
	if(PlayerCameraComponentRef && MasterGameInstance)
	{
		const bool r = IsPlayerLookingAtActor();

		if(!bLookingAt && r)
		{
			bLookingAt = true;
			MasterGameInstance->AddInteractionMessage(InteractionMessage);
			MasterGameInstance->SetSelectedInteractionComponent(this);
		}
		else if(bLookingAt && !r)
		{
			bLookingAt = false;
			MasterGameInstance->RemoveInteractionMessage();
			MasterGameInstance->ClearSelectedInteractionComponent();
		}
	}

}

bool UGSPInteractionComponent::IsPlayerLookingAtActor()
{
	if(PlayerCameraComponentRef && GetOwner())
	{
		const float a = FMath::Abs(PlayerCameraComponentRef->GetComponentRotation().Yaw);

		const float b = FMath::Abs(UKismetMathLibrary::FindLookAtRotation(PlayerCameraComponentRef->GetComponentLocation(), GetOwner()->GetActorLocation()).Yaw);

		return UKismetMathLibrary::InRange_FloatFloat(a, b - LookAngleTolerance, b + LookAngleTolerance);
	}
	return false;
}
