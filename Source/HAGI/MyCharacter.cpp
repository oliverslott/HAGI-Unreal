// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

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

void AMyCharacter::AddScore(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	Score += Amount;
	OnScoreChanged.Broadcast(Score);
	UpdateScoreHud();
}

void AMyCharacter::UpdateScoreHud()
{
	TArray<UUserWidget*> HudWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, HudWidgets, UUserWidget::StaticClass(), false);

	for (UUserWidget* Widget : HudWidgets)
	{
		if (!Widget || !Widget->GetClass()->GetPathName().Contains(TEXT("HUD_PLAYER")))
		{
			continue;
		}

		if (UTextBlock* ScoreText = Cast<UTextBlock>(Widget->WidgetTree->FindWidget(TEXT("Score"))))
		{
			ScoreText->SetText(FText::Format(
				NSLOCTEXT("HAGI", "ScoreFormat", "Score: {0}"),
				FText::AsNumber(Score)));
		}
	}
}
