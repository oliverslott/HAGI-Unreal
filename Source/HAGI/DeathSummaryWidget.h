#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathSummaryWidget.generated.h"

UCLASS()
class HAGI_API UDeathSummaryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetFinalScore(int32 InFinalScore);

protected:
	virtual void NativeOnInitialized() override;

private:
	void UpdateScoreText();

	int32 FinalScore = 0;

	UPROPERTY(Transient)
	TObjectPtr<class UTextBlock> ScoreText;
};
