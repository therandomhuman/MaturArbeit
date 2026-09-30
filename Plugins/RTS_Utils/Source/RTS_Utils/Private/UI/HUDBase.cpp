// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDBase.h"
#include "Blueprint/UserWidget.h"

#include "RTS_Unit_Base.h"
#include "DataAssets/UnitDataBase.h"

#include "UI/GeneralWidget.h"

void AHUDBase::AssignUnit(AActor* NewSelected)
{

	SelectedUnit = Cast<ARTS_Unit_Base>(NewSelected);//I need to cast the general aactor to my base class I can access my stuff

	MainWidget->SetVisibility(SelectedUnit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);//cool operator shenaging I remembered I can do, just cleaner than if stuff
	
	UpdateWidgets();
}

void AHUDBase::ToggleGameStateScreen()
{
	LoseScreenWidget->SetVisibility(ESlateVisibility::Visible);
}

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();

	// Grab a reference to the current world
	World = GetWorld();
	checkf(World, TEXT("Failed to reference world."));//useful debug tool that doesn't get compiled in actual shipping builds, I'm a lazy bum though so I rarely use em



	MainWidget = CreateWidget<UGeneralWidget>(World, MainUIWidgetClass);
	MainWidget->AddToViewport();
	MainWidget->SetVisibility(ESlateVisibility::Collapsed);

	LoseScreenWidget = CreateWidget<UUserWidget>(World, LoseScreenUIWidgetClass);
	LoseScreenWidget->AddToViewport();
	LoseScreenWidget->SetVisibility(ESlateVisibility::Collapsed);




	UpdateWidgets();
}

void AHUDBase::EndPlay(const EEndPlayReason::Type EndPlayReasn)
{
	Super::EndPlay(EndPlayReasn);
	//clean up delegates of the stuff here
}



void AHUDBase::UpdateWidgets()//this is specifically for general updates, meaning specific changes are handdled by delegates, it's cleaner that way
{
	if (!SelectedUnit)//if I don't have anything selected there is no point updating
	{
		return;
	}
	MainWidget->UpdateEntireUI(SelectedUnit);
	//SelectedUnit->UnitType->GetDefaultObject<UUnitDataBase>();


}

