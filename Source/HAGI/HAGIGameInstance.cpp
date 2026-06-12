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

void UHAGIGameInstance::RecordPlayerDeath(int32 FinalScore)
{
	LastScore = FMath::Max(0, FinalScore);
	bDeathSummaryPending = true;
}

void UHAGIGameInstance::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!bDeathSummaryPending || !LoadedWorld || !LoadedWorld->GetMapName().Contains(TEXT("Lvl_Menu")))
	{
		return;
	}

	LoadedWorld->GetTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(this, &UHAGIGameInstance::ShowDeathSummary, LoadedWorld));
}

void UHAGIGameInstance::ShowDeathSummary(UWorld* LoadedWorld)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(LoadedWorld, 0);
	if (!PlayerController)
	{
		return;
	}

	DeathSummaryWidget = CreateWidget<UDeathSummaryWidget>(PlayerController, UDeathSummaryWidget::StaticClass());
	if (DeathSummaryWidget)
	{
		DeathSummaryWidget->SetFinalScore(LastScore);
		DeathSummaryWidget->AddToViewport(100);
		bDeathSummaryPending = false;
	}
}
