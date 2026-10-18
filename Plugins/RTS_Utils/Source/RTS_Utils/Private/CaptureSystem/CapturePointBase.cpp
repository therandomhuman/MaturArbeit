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

	if (CaptorTag == OwningTeam || CaptorTag == LastOwner)
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

	FriendlyUnits.RemoveAll([](const TWeakObjectPtr<AActor>& Soldier)
		{
			return !Soldier.IsValid();
		});
	EnemyUnits.RemoveAll([](const TWeakObjectPtr<AActor>& Soldier)
		{
			return !Soldier.IsValid();
		});
	int32 NumEnemy = EnemyUnits.Num();
	int32 NumFriend = FriendlyUnits.Num();



	int32 CapFactor = 1;
	int32 PointLoss;
	if (NumEnemy > NumFriend)
	{
		CapFactor *= -1;
		PointLoss = NumEnemy - NumFriend;
		
	}
	else if (NumEnemy < NumFriend)
	{
		PointLoss = NumFriend - NumEnemy;
	}
	else
	{
		return; //there is no sidde with more folks, so no capture progress
	}

	if (OwningTeam == FGameplayTag::EmptyTag)
	{
		ICaptureInterface* PotentialCaptore = Cast<ICaptureInterface>(NumEnemy > NumFriend ? EnemyUnits[0] : FriendlyUnits[0]);
		if (!PotentialCaptore)//just in case the cast somehow failed, though it really shouldn't
		{
			return;
		}
		FGameplayTag WinningTeam = PotentialCaptore->GetCaptorTeam();
		if (CurrentlyAtCapAdvantage == WinningTeam || CurrentlyAtCapAdvantage == FGameplayTag::EmptyTag)
		{
			CapFactor *= -1;
		}
		//for chaning capadvantage
		if (CurrentlyAtCapAdvantage != WinningTeam && CurrentCapLevel <= KINDA_SMALL_NUMBER || CurrentlyAtCapAdvantage == FGameplayTag::EmptyTag)
		{
			CurrentlyAtCapAdvantage = WinningTeam;
		}

	}



	PointLoss = UKismetMathLibrary::Clamp(PointLoss, 0, 10);
	//so the more units the side has compared to other the faster it is changed, clamped to a max of 2x the default capture speed. 
	//example you have 2 mnore enemies than friendles, so 2/10 = 0.2+1 = 1.2x the capture speed
	float PointLossFloat = 1.f + static_cast<float>(PointLoss) / 10.f;
	CurrentCapLevel += PointLossFloat*CapFactor;
	CurrentCapLevel = FMath::Clamp(CurrentCapLevel, 0.f, MaxCapLevel);
	if (CurrentCapLevel <= KINDA_SMALL_NUMBER && !CurrentlyAtCapAdvantage.IsValid() || CurrentCapLevel == MaxCapLevel && CurrentlyAtCapAdvantage.IsValid())
	{
		RecalcPointOwnership();
	}
}

void ACapturePointBase::RecalcPointOwnership()
{

	if (OwningTeam != FGameplayTag::EmptyTag)
	{
		LastOwner = OwningTeam;
		OwningTeam = FGameplayTag::EmptyTag;
	}
	else
	{
		
		OwningTeam = CurrentlyAtCapAdvantage;
		if (LastOwner == OwningTeam)
		{
			return;
		}
		CaptureDelegate.Broadcast(CurrentlyAtCapAdvantage);
		CurrentlyAtCapAdvantage = FGameplayTag::EmptyTag;
		TArray<TWeakObjectPtr<AActor>> Temp = EnemyUnits;
		EnemyUnits = FriendlyUnits;
		FriendlyUnits = Temp;
	}
}



// Called every frame
void ACapturePointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

