// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/SpringArmComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS()
class HAGI_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	float AimPitch = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Aim")
	float CalculateAimPitch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomedDistance = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomSpeed = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void StartZoom();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void StopZoom();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount = 1);

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const { return Score; }

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoomToZoom;

private:
	float DefaultArmLength = 400.0f;
	float isZooming = false;

	void UpdateCameraZoom(float DeltaTime);
	void UpdateScoreHud();
};
