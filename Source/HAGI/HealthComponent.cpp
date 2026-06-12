// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "HAGIGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "GameFramework/Pawn.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamageAmount(float Amount)
{
	if (Amount <= 0.0f || IsDead())
	{
		return;
	}

	const float PreviousHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, MaxHealth);

	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (PreviousHealth > 0.0f && IsDead() && OwnerPawn && OwnerPawn->IsPlayerControlled())
	{
		if (const AMyCharacter* Player = Cast<AMyCharacter>(OwnerPawn))
		{
			if (UHAGIGameInstance* GameInstance = Cast<UHAGIGameInstance>(GetWorld()->GetGameInstance()))
			{
				GameInstance->RecordPlayerDeath(Player->GetScore());
			}
		}
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (PreviousHealth > 0.0f && IsDead())
	{
		OnDeath.Broadcast();

		if (OwnerPawn && !OwnerPawn->IsPlayerControlled())
		{
			if (AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
			{
				Player->AddScore();
			}
		}
	}
}

void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0.0f || IsDead())
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

