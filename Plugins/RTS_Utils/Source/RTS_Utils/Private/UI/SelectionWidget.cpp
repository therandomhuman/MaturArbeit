// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectionWidget.h"
#include "UI/HealthBarWidget.h"

#include "RTS_Unit_Base.h"
#include "Utilities/CommonUnitDataTypes.h"

#include "DataAssets/UnitDataBase.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"



void USelectionWidget::UpdateEntireUI(ARTS_Unit_Base* Unit)
{
	
	UnitInformation->SetText(Unit->UnitType->GetDefaultObject<UUnitDataBase>()->UnitInformation);
	UnitImage->SetBrushFromTexture(Unit->UnitType->GetDefaultObject<UUnitDataBase>()->UnitImage);

	HealthBarWidgets.Empty();
	TArray<UWidget*> Widgets = HealthBarContainer->GetAllChildren();
	int32 index = 0;
	const TArray<FSoldierPropertyStruct> Unitsolddiers = Unit->GetUnitdata();
	for (auto Widget : Widgets)
	{
		
		UHealthBarWidget* healthwidg = Cast<UHealthBarWidget>(Widget);
		if (!healthwidg)
		{
			index++;
			continue;
		}
		if (Unitsolddiers.Num() > index)
		{
			healthwidg->SetVisibility(ESlateVisibility::HitTestInvisible);
			HealthBarWidgets.Add(healthwidg);
			healthwidg->OnStatUpdated(1, Unitsolddiers[index].UnitHealth, 100);
		}
		else
		{
			healthwidg->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		index++;
	}
}

void USelectionWidget::UpdateHealth(int32 index, int32 Health)
{
	HealthBarWidgets[index]->OnStatUpdated(1, Health, 100);
}


