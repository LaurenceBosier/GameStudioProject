// Game Studio Project Team F 2023


#include "Gameplay/GSPHealthComponent.h"


void UGSPHealthComponent::BeginPlay()
{
	CurrentHealth = MaxHealth;

	Super::BeginPlay();
}

bool UGSPHealthComponent::DealDamage(AActor* Instigator, int DamageAmount)
{
	//Subsiquent calls to deal damage after death will be disregarded 
	if(bIsDead)
	{
		return true;
	}

	//Broadcast the event to blueprints
	UGSPHealthComponent::OnTakeDamage.Broadcast<AActor*, int>(Instigator, DamageAmount);

	//If the actor is still alive 
	if((CurrentHealth -= DamageAmount) > 0)
	{
		return false; 
	}

	//If the actor was killed by the damage
	CurrentHealth = 0;

	//Set the actors state to be dead
	bIsDead = true; 

	//Broadcast OnDeath event to blueprints
	UGSPHealthComponent::OnDeath.Broadcast<AActor*>(Instigator);

	return true;
}

bool UGSPHealthComponent::AddHealth(int HealthToAdd)
{

	//Disregard adding health if the actor is in the dead state
	if(bIsDead)
	{
		return false;
	}

	//Broadcast the event to blueprints
	UGSPHealthComponent::OnHeal.Broadcast<int>(HealthToAdd);


	//If the player is fully healed
	if((CurrentHealth += HealthToAdd) >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
		return true; 
	}

	return false;
}
