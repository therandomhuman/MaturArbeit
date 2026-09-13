// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "Perception/AIPerceptionComponent.h"

#include "RTS_Unit_Base.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	AiPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this,&AAIControllerBase::PerceptionTargetUpdate);
	
	//get owned pawn and their tags
	PossesedPawn = GetPawn();
	PossedUnit = Cast<ARTS_Unit_Base>(PossesedPawn);
	IGameplayTagAssetInterface* SelectedActorTagInter = Cast<IGameplayTagAssetInterface>(PossesedPawn);
	if (!SelectedActorTagInter)
	{
		return;
	}
	SelectedActorTagInter->GetOwnedGameplayTags(OwnerTags);
}

void AAIControllerBase::PerceptionTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Name %s") ,*Actor->GetName())
	IGameplayTagAssetInterface* SelectedActorTagInter = Cast<IGameplayTagAssetInterface>(Actor);
	if (!SelectedActorTagInter)
	{
		return;
	}
	FGameplayTagContainer TargetTags;
	SelectedActorTagInter->GetOwnedGameplayTags(TargetTags);

	if (TargetTags.GetByIndex(0).RequestDirectParent() != OwnerTags.GetByIndex(0).RequestDirectParent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Perceived evil enemy, grrrrrrr"));
		if (PossedUnit != nullptr)
		{
			if (!PossedUnit->GetTarget())
			{
				PossedUnit->SetTarget(Actor);
			}
		}
		
	}

}


AAIControllerBase::AAIControllerBase()
{

	AiPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Ai Perception Component"));



}