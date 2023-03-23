// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameStudioProjectCharacter.generated.h"


UCLASS(config=Game)
class AGameStudioProjectCharacter : public ACharacter
{

protected:

	GENERATED_BODY()

	AGameStudioProjectCharacter();

	// To add mapping context
	virtual void BeginPlay();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
;

protected:

	/**
	 * @brief Checks if the input scale is out of the defined stick dead-zone
	 * @return True if InScale is in the axis dead-zone
	 */
	FORCEINLINE bool IsAxisDeadZone(const float InScale);

	/** Called for forwards/backward input */
	void MoveForward(float InValue);

	/** Called for side to side input */
	void MoveRight(float InValue);

	/** Called for sprint down input */
	void StartSprint();

	/** Called for sprint up input */
	void StopSprint();

	/** Called for crouch down input */
	void StartCrouch(); //Todo if pressed in quick succession you should roll and return to previous state

	/** Called for crouch up input */
	void StopCrouch();

	/** Called for block down input */
	void StartBlock();

	/** Called for block up input */
	void StopBlock();

	/** Called for attack down input */
	void TryAttack();

	/** Called for interaction actions */
	void TryInteract();

	/** Called for interaction actions */
	void TryCombatLock();

	/** Called for interaction actions */
	void ToggleInventory();

	/** Called for interaction actions */
	void ToggleMap();


public:

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats")
	float DefaultWalkSpeed = 425;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats")
	float SprintSpeed = 630;


	UPROPERTY()
	class UGSPMasterGameInstance* MasterGameInstanceRef;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Players health component **/
	FORCEINLINE class UGSPHealthComponent* GetPlayerHealthComponent() const { return PlayerHealthComponent; }

private:

	/* Player Health Component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	class UGSPHealthComponent* PlayerHealthComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};

