#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HAGIGameInstance.generated.h"

UCLASS()
class HAGI_API UHAGIGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	void RecordPlayerDeath(int32 FinalScore);

private:
	void HandlePostLoadMap(UWorld* LoadedWorld);
	void ShowDeathSummary(UWorld* LoadedWorld);

	bool bDeathSummaryPending = false;
	int32 LastScore = 0;

	UPROPERTY(Transient)
	TObjectPtr<class UDeathSummaryWidget> DeathSummaryWidget;
};
