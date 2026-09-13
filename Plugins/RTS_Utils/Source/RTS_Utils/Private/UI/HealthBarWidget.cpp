// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UHealthBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UpdateWidget();
}

void UHealthBarWidget::OnStatUpdated(int32 OldValue, int32 NewValue, int32 MaxValue)
{
	CurrentPrecentage = FMath::Clamp(static_cast<float>(NewValue) / static_cast<float>(MaxValue), 0.f, 1.f);
	CurrentValue = static_cast<float>(NewValue);
	UpdateWidget();
}

void UHealthBarWidget::UpdateWidget()
{


	FSlateChildSize EmptySize = FSlateChildSize(ESlateSizeRule::Fill);
	EmptySize.Value = 1.f - CurrentPrecentage;
	FSlateChildSize FilledSize = FSlateChildSize(ESlateSizeRule::Fill);
	FilledSize.Value =CurrentPrecentage;

	if (PrecentFilled == nullptr)
		return;

	if (UHorizontalBoxSlot* FilledSlot = Cast<UHorizontalBoxSlot>(PrecentFilled->Slot))
		FilledSlot->SetSize(FilledSize);

	if (UHorizontalBoxSlot* EmptySlot = Cast<UHorizontalBoxSlot>(PrecentEmpty->Slot))
		EmptySlot->SetSize(EmptySize);

	if (CurrentPrecentage <= 0.f)
		SetVisibility(ESlateVisibility::Collapsed);

}


#if WITH_EDITOR
void UHealthBarWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateWidget();

}
#endif