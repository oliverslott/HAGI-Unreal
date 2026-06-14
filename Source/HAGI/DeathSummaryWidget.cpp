#include "DeathSummaryWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UDeathSummaryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	WidgetTree->RootWidget = RootCanvas;

	UVerticalBox* TextContainer = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	UCanvasPanelSlot* ContainerSlot = RootCanvas->AddChildToCanvas(TextContainer);
	ContainerSlot->SetAnchors(FAnchors(0.5f, 0.16f));
	ContainerSlot->SetAlignment(FVector2D(0.5f, 0.0f));
	ContainerSlot->SetAutoSize(true);

	UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	TitleText->SetText(NSLOCTEXT("HAGI", "YouDied", "You died"));
	TitleText->SetJustification(ETextJustify::Center);
	TitleText->SetColorAndOpacity(FSlateColor(FLinearColor(0.9f, 0.08f, 0.08f)));
	FSlateFontInfo TitleFont = TitleText->GetFont();
	TitleFont.Size = 48;
	TitleText->SetFont(TitleFont);
	TextContainer->AddChildToVerticalBox(TitleText);

	ScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	ScoreText->SetJustification(ETextJustify::Center);
	FSlateFontInfo ScoreFont = ScoreText->GetFont();
	ScoreFont.Size = 28;
	ScoreText->SetFont(ScoreFont);
	UVerticalBoxSlot* ScoreSlot = TextContainer->AddChildToVerticalBox(ScoreText);
	ScoreSlot->SetPadding(FMargin(0.0f, 8.0f, 0.0f, 0.0f));

	UpdateScoreText();
}

void UDeathSummaryWidget::SetFinalScore(int32 InFinalScore)
{
	FinalScore = FMath::Max(0, InFinalScore);
	UpdateScoreText();
}

void UDeathSummaryWidget::UpdateScoreText()
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::Format(
			NSLOCTEXT("HAGI", "FinalScoreFormat", "Score: {0}"),
			FText::AsNumber(FinalScore)));
	}
}
