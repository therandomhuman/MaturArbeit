// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "Perception/AIPerceptionComponent.h"

#include "RTS_Unit_Base.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AiPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::PerceptionTargetUpdate);

	PossesedPawn = InPawn;
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

	IGameplayTagAssetInterface* SelectedActorTagInter = Cast<IGameplayTagAssetInterface>(Actor);
	if (!SelectedActorTagInter)
	{
		return;
	}
	FGameplayTagContainer TargetTags;
	SelectedActorTagInter->GetOwnedGameplayTags(TargetTags);

	if (TargetTags.GetByIndex(0).RequestDirectParent() != OwnerTags.GetByIndex(0).RequestDirectParent())
	{

		if (PossedUnit != nullptr)
		{
			if (Stimulus.WasSuccessfullySensed() == true)
			{
				SensedEnemyDelegate.Broadcast(Actor, true);
				if (!PossedUnit->GetTarget())
				{
					PossedUnit->SetTarget(Actor);
				}
			}else if (PossedUnit->GetTarget() and Stimulus.WasSuccessfullySensed() == false)
			{
				SensedEnemyDelegate.Broadcast(Actor, false);
				PossedUnit->RemoveTarget(Actor);
			}
			
		}
		
	}

}


AAIControllerBase::AAIControllerBase()
{

	AiPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Ai Perception Component"));



}