#include "HAGIGameInstance.h"

#include "DeathSummaryWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"

void UHAGIGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UHAGIGameInstance::HandlePostLoadMap);
}

void UHAGIGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Shutdown();
}

void UHAGIGameInstance::UpdateCurrentScore(int32 CurrentScore)
{
	LastScore = FMath::Max(0, CurrentScore);
}

void UHAGIGameInstance::HandlePostLoadMap(UWorld* LoadedWorld)
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
		this, &UHAGIGameInstance::ShowDeathSummary, LoadedWorld);
	LoadedWorld->GetTimerManager().SetTimer(
		DeathSummaryRetryTimer, ShowSummaryDelegate, 0.1f, true, 0.0f);
}

void UHAGIGameInstance::ShowDeathSummary(UWorld* LoadedWorld)
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
