#include "CustomGameInstance.h"

#include "DeathSummaryWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"

void UCustomGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCustomGameInstance::HandlePostLoadMap);
}

void UCustomGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Shutdown();
}

void UCustomGameInstance::UpdateCurrentScore(int32 CurrentScore)
{
	LastScore = FMath::Max(0, CurrentScore);
}

void UCustomGameInstance::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!LoadedWorld)
	{
		return;
	}

	const FString MapName = LoadedWorld->GetMapName();
	if (MapName.Contains(TEXT("Lvl_ThirdPerson")))
	{
		bGameplaySessionActive = true;
		bDeathSummaryPending = false;
		LastScore = 0;
	}
	else if (MapName.Contains(TEXT("Lvl_Menu")) && bGameplaySessionActive)
	{
		bGameplaySessionActive = false;
		bDeathSummaryPending = true;
	}

	if (!bDeathSummaryPending || !MapName.Contains(TEXT("Lvl_Menu")))
	{
		return;
	}

	FTimerDelegate ShowSummaryDelegate = FTimerDelegate::CreateUObject(
		this, &UCustomGameInstance::ShowDeathSummary, LoadedWorld);
	LoadedWorld->GetTimerManager().SetTimer(
		DeathSummaryRetryTimer, ShowSummaryDelegate, 0.1f, true, 0.0f);
}

void UCustomGameInstance::ShowDeathSummary(UWorld* LoadedWorld)
{
	if (!bDeathSummaryPending || !LoadedWorld || !LoadedWorld->GetMapName().Contains(TEXT("Lvl_Menu")))
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(LoadedWorld, 0);
	if (!PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}

	DeathSummaryWidget = CreateWidget<UDeathSummaryWidget>(PlayerController, UDeathSummaryWidget::StaticClass());
	if (DeathSummaryWidget)
	{
		DeathSummaryWidget->SetFinalScore(LastScore);
		DeathSummaryWidget->AddToViewport(100);
		bDeathSummaryPending = false;
		LoadedWorld->GetTimerManager().ClearTimer(DeathSummaryRetryTimer);
	}
}
