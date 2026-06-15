#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

UCLASS()
class HAGI_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	void UpdateCurrentScore(int32 CurrentScore);

private:
	void HandlePostLoadMap(UWorld* LoadedWorld);
	void ShowDeathSummary(UWorld* LoadedWorld);

	bool bDeathSummaryPending = false;
	bool bGameplaySessionActive = false;
	int32 LastScore = 0;

	UPROPERTY(Transient)
	TObjectPtr<class UDeathSummaryWidget> DeathSummaryWidget;

	FTimerHandle DeathSummaryRetryTimer;
};
