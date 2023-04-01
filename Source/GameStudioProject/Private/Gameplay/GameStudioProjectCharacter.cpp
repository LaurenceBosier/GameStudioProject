// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameStudioProject/Public/Gameplay/GameStudioProjectCharacter.h"

#include <complex.h>

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameCore/GSPFunctionLibrary.h"
#include "GameCore/GSPMasterGameInstance.h"
#include "Gameplay/GSPHealthComponent.h"
#include "Gameplay/GSPInventoryComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// AGameStudioProjectCharacter

AGameStudioProjectCharacter::AGameStudioProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 385;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->Mass = 75;
	GetCharacterMovement()->bPushForceScaledToMass = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	Player3DRenderCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Player3DRenderCamera"));
	Player3DRenderCamera->SetupAttachment(GetMesh());
	Player3DRenderCamera->ProjectionType = ECameraProjectionMode::Orthographic;
	Player3DRenderCamera->OrthoWidth = 120;
	Player3DRenderCamera->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
	Player3DRenderCamera->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	Player3DRenderCamera->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

	//Create 3D render camera for player inventory 
	Player3DRenderCamera->bCaptureEveryFrame = false;
	Player3DRenderCamera->bCaptureOnMovement = false;

	//Create player health component
	PlayerHealthComponent = CreateDefaultSubobject<UGSPHealthComponent>(TEXT("PlayerHealth"));

	//Creates player inventory component
	PlayerInventoryComponent = CreateDefaultSubobject<UGSPInventoryComponent>(TEXT("PlayerInventory"));


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGameStudioProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Set the master game instance ref; 
	MasterGameInstanceRef = UGSPFunctionLibrary::GetGSPGameInstance(this);

	//Set the players walk speed to the DefaultWalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

	//Make the Player3DRenderCamera only render this actor
	Player3DRenderCamera->ShowOnlyActors = { this };

	//Try add player HUD to screen //Todo move this to show/hide game instance creates it at init and sets to invis
	if(MasterGameInstanceRef)
	{
		MasterGameInstanceRef->TryCreateGamePlayHUDWidget();
	}

}

bool AGameStudioProjectCharacter::IsAxisDeadZone(const float InScale)
{
	if(InScale > 0.055 || InScale < -0.055)
	{
		return false;
	}
	return true;
}

void AGameStudioProjectCharacter::MoveForward(float InValue)
{
	if ((Controller != nullptr) && (InValue != 0.0f) && !IsAxisDeadZone(InValue))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InValue);
	}
}

void AGameStudioProjectCharacter::MoveRight(float InValue)
{
	if ( (Controller != nullptr) && (InValue != 0.0f) && !IsAxisDeadZone(InValue))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, InValue);
	}
}

void AGameStudioProjectCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AGameStudioProjectCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void AGameStudioProjectCharacter::StartCrouch()
{
}

void AGameStudioProjectCharacter::StopCrouch()
{
}

void AGameStudioProjectCharacter::StartBlock()
{
}

void AGameStudioProjectCharacter::StopBlock()
{
}

void AGameStudioProjectCharacter::TryAttack()
{
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGameStudioProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	check(PlayerInputComponent);

	/* Action Mappings */

	//Jump binding
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGameStudioProjectCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGameStudioProjectCharacter::StopJumping);

	//Sprint start / stop binding
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AGameStudioProjectCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AGameStudioProjectCharacter::StopSprint);

	//Crouch / Roll, start / stop binding 
	PlayerInputComponent->BindAction("Crouch / Roll", IE_Pressed, this, &AGameStudioProjectCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch / Roll", IE_Released, this, &AGameStudioProjectCharacter::StopCrouch);

	//Attack binding
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGameStudioProjectCharacter::TryAttack);

	//Combat Lock binding
	PlayerInputComponent->BindAction("Combat Lock", IE_Pressed, this, &AGameStudioProjectCharacter::TryCombatLock);

	//Interact binding
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGameStudioProjectCharacter::TryInteract);

	//Toggle Inventory binding
	PlayerInputComponent->BindAction("Toggle Inventory", IE_Pressed, this, &AGameStudioProjectCharacter::ToggleInventory);

	//Toggle Map binding
	PlayerInputComponent->BindAction("Toggle Map", IE_Pressed, this, &AGameStudioProjectCharacter::ToggleMap);

	//Block start / stop binding
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AGameStudioProjectCharacter::StartBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AGameStudioProjectCharacter::StopBlock);


	/* Axis Mappings */
	
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGameStudioProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGameStudioProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGameStudioProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGameStudioProjectCharacter::LookUpAtRate);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGameStudioProjectCharacter::TryInteract()
{
	if(MasterGameInstanceRef)
	{
		MasterGameInstanceRef->TryInteractWithSelectedActor();
	}
}

void AGameStudioProjectCharacter::TryCombatLock()
{
	UE_LOG(LogTemp, Log, TEXT("Attempt Combat Lock, (work in progress)"));

	if(GetWorld())
	{

	}
}

//Todo remove this mess!
void AGameStudioProjectCharacter::ToggleInventory()
{
	if(!MasterGameInstanceRef)
	{
		return;
	}

	const auto widgetRef = MasterGameInstanceRef->GameMenuHUDInst;

	if(!widgetRef)
	{
		return;
	}

	APlayerController* pcRef = static_cast<APlayerController*>(GetController());

	if(!pcRef)
	{
		return;
	}

	if(widgetRef->GetVisibility() == ESlateVisibility::Hidden)
	{
		Player3DRenderCamera->bCaptureEveryFrame = true;
		Player3DRenderCamera->bCaptureOnMovement = true;

		FollowCamera->PostProcessSettings.bOverride_ReflectionMethod = true;
		FollowCamera->PostProcessSettings.bOverride_DynamicGlobalIlluminationMethod = true;
		FollowCamera->PostProcessSettings.ReflectionMethod = EReflectionMethod::None;
		FollowCamera->PostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::None;

		widgetRef->SetVisibility(ESlateVisibility::Visible);
		pcRef->SetShowMouseCursor(true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(pcRef, widgetRef, EMouseLockMode::LockInFullscreen);
		return;
	}

	FollowCamera->PostProcessSettings.bOverride_ReflectionMethod = true;
	FollowCamera->PostProcessSettings.bOverride_DynamicGlobalIlluminationMethod = true;
	FollowCamera->PostProcessSettings.ReflectionMethod = EReflectionMethod::Lumen;
	FollowCamera->PostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;

	Player3DRenderCamera->bCaptureEveryFrame = false;
	Player3DRenderCamera->bCaptureOnMovement = false;
	pcRef->SetShowMouseCursor(false);
	MasterGameInstanceRef->GameMenuHUDInst->SetVisibility(ESlateVisibility::Hidden);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(pcRef);
}

void AGameStudioProjectCharacter::ToggleMap()
{

}




