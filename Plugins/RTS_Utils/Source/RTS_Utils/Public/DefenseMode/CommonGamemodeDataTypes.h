

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonGamemodeDataTypes.generated.h"


class ARTS_Unit_Base;
UENUM()
enum EObjectiveType
{
	Capture
};

USTRUCT()
struct FObjective
{
	GENERATED_BODY()
	EObjectiveType ObjectiveType = EObjectiveType::Capture;

	AActor* RelevantObject = nullptr;
};

USTRUCT()
struct FAiUnitGroup
{
	//maybe store their contrller not their pawn since yknow
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ARTS_Unit_Base>> Units;
	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	FGameplayTag CurrentActivity;


};

USTRUCT()
struct FTrackedEnemy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TrackedUnit;
	UPROPERTY(EditAnywhere, Category = General, meta = (AllowPrivateAccess = "true"))
	TArray<ARTS_Unit_Base*> EngagedFriendlies;
};