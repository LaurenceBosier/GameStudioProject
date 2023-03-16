// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "GameCore/GSPActorComponentBase.h"
#include "GSPHealthComponent.generated.h"


//Todo add a bone damage multiplier. (By default multiply all damage by 1 unless the bone has a damage value mapped to it).
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(GSP), 
	meta=(BlueprintSpawnableComponent))
class GAMESTUDIOPROJECT_API UGSPHealthComponent : public UGSPActorComponentBase
{
	GENERATED_BODY()


public:

	virtual void BeginPlay() override;

	/**
	 * @brief Applies damage to the component
	 * @param InInstigator The actor dealing the damage 
	 * @param InDamageAmount The amount of damage the single attack will deal 
	 * @return True if the actor was killed by the dealt damage 
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (DefaultToSelf = InInstigator))
	bool DealDamage(AActor* InInstigator, int InDamageAmount);

	/**
	 * @brief Heals the actor by a specified amount 
	 * @param InHealthToAdd The amount of health to increment by
	 * @return True if the actor is at full health after the heal
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	bool AddHealth(int InHealthToAdd);

	/**
	 * @brief Updates if the actor is currently invincible
	 * @param InNewInvincibleState True if the actor should be made invincible
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SetInvincibility(bool InNewInvincibleState);

	/**
	 * @brief Getter function for current health
	 * @return The actors current health
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	FORCEINLINE int GetCurrentHealth() const { return CurrentHealth; }

	/**
	 * @brief Getter function for max health
	 * @return The actors max health
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	FORCEINLINE int GetMaxHealth() const { return MaxHealth; }

	/**
	 * @brief Getter function to check if the actor is alive
	 * @return True if the actor is alive
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	FORCEINLINE bool IsAlive() const { return !bIsDead; }

	/**
	 * @brief Getter function to check if the actor is invincible
	 * @return True of the actor is invincible
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	FORCEINLINE bool IsInvincible() const { return bInvincible; }

public:

	//True if the actor can take damage but can't die. 
	UPROPERTY(EditAnywhere, Category = "Health Component")
	bool bInvincible = false;

	//Actors max health
	UPROPERTY(EditAnywhere, Category = "Health Component", meta = (EditCondition = "!bInvincible", EditConditionHides))
	int MaxHealth { 100 };

private:

	//The current health of the actor
	UPROPERTY()
	int CurrentHealth;

	//The actors state (If it's currently alive or not) 
	bool bIsDead = false; 
	
	/* Declare dynamic multi-cast delegates */

	//OnDeath Delegate, called when the health reaches or falls below zero. 
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnDeathSignature,  UGSPHealthComponent, OnDeath, AActor*, OutKiller);

	//OnTakeDamage, called when damage is applied to the actor.
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FOnTakeDamageSignature, UGSPHealthComponent, OnTakeDamage, AActor*, Instigator, int, OutDamageAmount);

	//OnHeal, called when the actor receives health
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnHealSignature, UGSPHealthComponent, OnHeal, int, HealAmount); 

	//OnNewInvincibleState, called when the actors invincible state is changed
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnNewInvincibleStateSignature, UGSPHealthComponent, OnNewInvincibleState, bool, OutNewInvincibleState); 

	/* End dynamic multi-cast delegates */

public:

	/* Functions bound in blueprints */

	//OnDeath Delegate, called when the health reaches or falls below zero. 
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnDeathSignature OnDeath;

	//OnTakeDamage, called when damage is applied to the actor.
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnTakeDamageSignature OnTakeDamage;

	//OnHeal, called when the actor receives health
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnHealSignature OnHeal;
	
	//OnNewInvincibleState, called when the actors invincible state is changed
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnNewInvincibleStateSignature OnNewInvincibleState;

	/* End of function bindings */

	
};


