// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPHealthComponent.h"


void UGSPHealthComponent::BeginPlay()
{
	CurrentHealth = MaxHealth;

	Super::BeginPlay();
}

bool UGSPHealthComponent::DealDamage(AActor* InInstigator, int InDamageAmount)
{
	//Subsiquent calls to deal damage after death will be disregarded 
	if(bIsDead)
	{
		return true;
	}

	//Subtract health from actor
	CurrentHealth -= InDamageAmount;

	//Broadcast the event to blueprints
	UGSPHealthComponent::OnTakeDamage.Broadcast<AActor*, int>(InInstigator, InDamageAmount);

	//If the actor is still alive 
	if(bInvincible || CurrentHealth > 0)
	{
		//Return because the actor is still alive
		return false; 
	}

	//If the actor was killed by the damage
	CurrentHealth = 0;

	//Set the actors state to be dead
	bIsDead = true; 

	//Broadcast OnDeath event to blueprints
	UGSPHealthComponent::OnDeath.Broadcast<AActor*>(InInstigator);

	//Return true (actor is dead)
	return true;
}

bool UGSPHealthComponent::AddHealth(int InHealthToAdd)
{

	//Disregard adding health if the actor is in the dead state
	if(bIsDead)
	{
		return false;
	}

	//Broadcast the event to blueprints
	UGSPHealthComponent::OnHeal.Broadcast<int>(InHealthToAdd);


	//If the player is fully healed
	if((CurrentHealth += InHealthToAdd) >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
		return true; 
	}

	return false;
}

void UGSPHealthComponent::SetInvincibility(bool InNewInvincibleState)
{
	bInvincible = InNewInvincibleState;
	//Broadcast to the blueprint the actors new invincibility state 
	OnNewInvincibleState.Broadcast<bool>(InNewInvincibleState);
}

