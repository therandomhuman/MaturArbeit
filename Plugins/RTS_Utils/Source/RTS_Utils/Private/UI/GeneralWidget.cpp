// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GeneralWidget.h"

#include "UI/SelectionWidget.h"

#include "RTS_Unit_Base.h"

void UGeneralWidget::UpdateEntireUI(ARTS_Unit_Base* Unit)
{
	SelectedUnitWidget->UpdateEntireUI(Unit);

	Unit->UnitHealthDelegate.AddUObject(SelectedUnitWidget, &USelectionWidget::UpdateHealth);
}
