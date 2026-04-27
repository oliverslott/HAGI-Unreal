// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraBoomToZoom = FindComponentByClass<USpringArmComponent>();

	if (CameraBoomToZoom)
	{
		DefaultArmLength = CameraBoomToZoom->TargetArmLength;
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimPitch = CalculateAimPitch();
	UpdateCameraZoom(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyCharacter::CalculateAimPitch()
{
	if (!Controller)
	{
		return 0.0f;
	}

	float pitch = FRotator::NormalizeAxis(Controller->GetControlRotation().Pitch);

	float NormalizedPitch = pitch / 90.0f;

	return FMath::Clamp(NormalizedPitch, -1.0f, 1.0f);
}

void AMyCharacter::UpdateCameraZoom(float DeltaTime)
{
	if (!CameraBoomToZoom)
	{
		return;
	}

	const float TargetArmLength = isZooming ? ZoomedDistance : DefaultArmLength;

	CameraBoomToZoom->TargetArmLength = FMath::FInterpTo(CameraBoomToZoom->TargetArmLength, TargetArmLength, DeltaTime, ZoomSpeed);
}

void AMyCharacter::StartZoom()
{
	isZooming = true;
}

void AMyCharacter::StopZoom()
{
	isZooming = false;
}