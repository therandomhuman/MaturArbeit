// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureSystem/CapturePointBase.h"
#include "Components/SphereComponent.h"
#include "Utilities/CaptureInterface.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACapturePointBase::ACapturePointBase()
{





	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Capture Volume"));
	RootComponent = SphereComponent;
}

// Called when the game starts or when spawned
void ACapturePointBase::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACapturePointBase::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACapturePointBase::EndOverlap);
}

void ACapturePointBase::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	//make sure the overlapped unit can actually capture(I know it's a unit since I already use object collision channels to filter all others out)
	ICaptureInterface* PotentialCaptore = Cast<ICaptureInterface>(OtherActor);
	if (!PotentialCaptore)
	{
		return;
	}
	FGameplayTag CaptorTag = PotentialCaptore->GetCaptorTeam();

	if (CaptorTag == OwningTeam)
	{
		FriendlyUnits.AddUnique(OtherActor);
	}
	else
	{
		EnemyUnits.AddUnique(OtherActor);
	}

	if (!GetWorldTimerManager().IsTimerActive(CaptureTimerHandle))
	{
		GetWorldTimerManager().SetTimer(
			CaptureTimerHandle,
			this,
			&ACapturePointBase::UpdateCapture,
			CaptureInterval,
			true
		);
	}
}

void ACapturePointBase::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ICaptureInterface* PotentialCaptore = Cast<ICaptureInterface>(OtherActor);
	if (!PotentialCaptore)
	{
		return;
	}
	
		FriendlyUnits.Remove(OtherActor);
		EnemyUnits.Remove(OtherActor);

		if (FriendlyUnits.IsEmpty() and EnemyUnits.IsEmpty())
		{
			GetWorldTimerManager().ClearTimer(CaptureTimerHandle);
		}
}

void ACapturePointBase::UpdateCapture()
{
	for (auto& soldierptr: FriendlyUnits)
	{
		if (!soldierptr.IsValid())
		{
			FriendlyUnits.Remove(soldierptr);
		}
	}
	for (auto& soldierptr : EnemyUnits)
	{
		if (!soldierptr.IsValid())
		{
			EnemyUnits.Remove(soldierptr);
		}
	}

	int32 NumEnemy = EnemyUnits.Num();
	int32 NumFriend = FriendlyUnits.Num();
	//if the point has an owner these two, if it doesn't it needs to tick up
	if (NumEnemy > NumFriend)
	{
		int32 PointLoss = NumEnemy - NumFriend;
		PointLoss *= 2;
		PointLoss = UKismetMathLibrary::Clamp(PointLoss, 0, 10);
		//so the more enemies the point has compared to friendlies the faster it is lost, clamped to a max of 2x the default capture speed. 
		//example you have 2 mnore enemies than friendles, so 2/10 = 0.2+1 = 1.2x the capture speed
		float PointLossFloat = 1.f + static_cast<float>(PointLoss) / 10.f;
		CurrentCapLevel -= PointLossFloat;
		CurrentCapLevel = FMath::Clamp(CurrentCapLevel, 0.f, MaxCapLevel);
		if (CurrentCapLevel <= 0)
		{
			RecalcPointOwnership();
		}

	}
	else 
	{
		int32 PointLoss = NumEnemy - NumFriend;
		PointLoss *= 2;
		PointLoss = UKismetMathLibrary::Clamp(PointLoss, 0, -10);
		//so the more enemies the point has compared to friendlies the faster it is lost, clamped to a max of 2x the default capture speed. 
		//example you have 2 mnore enemies than friendles, so 2/10 = 0.2+1 = 1.2x the capture speed
		float PointLossFloat = -1.f + PointLoss / 10;
		CurrentCapLevel -= PointLossFloat;
		CurrentCapLevel = UKismetMathLibrary::Clamp(CurrentCapLevel, 0, MaxCapLevel);
		if (CurrentCapLevel <= 0)
		{
			RecalcPointOwnership();
		}
	}
}

void ACapturePointBase::RecalcPointOwnership(bool bEnemyCap)
{
	if (OwningTeam != FGameplayTag())
	{
		OwningTeam == FGameplayTag();
	}
	else
	{
		if (bEnemyCap)
		{
			ICaptureInterface* PotentialCaptore = Cast<ICaptureInterface>(EnemyUnits[0]);
			if (!PotentialCaptore)
			{
				return;
			}
			OwningTeam = PotentialCaptore->GetCaptorTeam();
			
		}
	}
}



// Called every frame
void ACapturePointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

