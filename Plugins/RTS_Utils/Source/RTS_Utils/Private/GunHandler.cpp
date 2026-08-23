// Fill out your copyright notice in the Description page of Project Settings.


#include "GunHandler.h"
#include "RTS_Unit_Base.h"
#include "Utilities/CommonUnitDataTypes.h"
#include "WeaponSystem/BaseProjectile.h"
#include  "Components//InstancedStaticMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGunHandler::UGunHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


	
}


// Called when the game starts
void UGunHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGunHandler::SetCanFire(FSoldierPropertyStruct& FiringSoldier)
{
//since these timer delegates don't really work with references I have to use my setter function, not too much of an issue, just a bit more verbose 
	FiringSoldier.SoldierWeapon.bCanFire = true;
	Cast<ARTS_Unit_Base>(GetOwner())->SetSoldierData(FiringSoldier.Index, FiringSoldier);
}

void UGunHandler::SetUpTarget(AActor* Target)
{
	//set up target
	IGameplayTagAssetInterface* SelectedActorTagInter = Cast<IGameplayTagAssetInterface>(Target);
	if (!SelectedActorTagInter)
	{
		return;
	}
	FGameplayTagContainer TargetTags;
	SelectedActorTagInter->GetOwnedGameplayTags(TargetTags);


	for (auto Tag : TargetTags)
	{
		if (Tag.MatchesTag(CompareTag))
		{
			if (Tag.GetTagLeafName() == FName("Unit"))
			{
				//cache this stuff later
				TargetUnit = Cast<ARTS_Unit_Base>(Target);
				TargetedISM  = TargetUnit->GetIsm();
				bIsUnit = true;
			}
			else
			{
	
			}
		}
	}
}

void UGunHandler::FireGun(FSoldierPropertyStruct& FiringSoldier,AActor* Target)
{


	FVector TargetPos;

			if (bIsUnit)
			{ 
			//cache this stuff later

				FTransform EnemyUnitPos;
				TargetedISM->GetInstanceTransform(UKismetMathLibrary::RandomIntegerInRange(0, TargetedISM->GetInstanceCount() - 1), EnemyUnitPos, true);
				TargetPos = EnemyUnitPos.GetLocation();
			}
			else
			{
				TargetPos = Target->GetActorLocation();
			}


	//spawning the projectile
	FActorSpawnParameters SpawnParams;
	ARTS_Unit_Base* Owner = Cast<ARTS_Unit_Base>(GetOwner());
	SpawnParams.Instigator = Owner;

	//get unit positon
	FTransform UnitTransform;
	Owner->GetIsm()->GetInstanceTransform(FiringSoldier.Index, UnitTransform, true);
	FVector UnitPos = UnitTransform.GetLocation() + FVector(0, 0, 25);


	//calculate right fly direction
	FVector DirectionVector =   TargetPos + FVector(0,0,5) - UnitPos;
	FRotator FlyDirection = UKismetMathLibrary::MakeRotFromXY(DirectionVector, DirectionVector);
	AActor* NewProjAc = GetWorld()->SpawnActor<ABaseProjectile>(FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->ProjectileVisual, UnitPos, FlyDirection, SpawnParams);

	if (NewProjAc == nullptr)
	{
		return;
	}
	ABaseProjectile* NewProjectile = Cast<ABaseProjectile>(NewProjAc);
	if (NewProjectile == nullptr)
	{
		return;
	}
	UWeaponData* DefObject = FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>();
	NewProjectile->SetUpNewProjectile(DefObject->MuzzleVelocity, DefObject->Gravity, DefObject->Damage);

	//fire visuals here
	FiringSoldier.SoldierWeapon.bCanFire = false;
	FiringSoldier.SoldierWeapon.CurrentAmmo -= 1;

	//set up timer so that the reset function gets called asynchronously from the main thread
	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, FName("SetCanFire"), FiringSoldier);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->FireRate, false);
}



void UGunHandler::Reload(FSoldierPropertyStruct& FiringSoldier)
{
	FiringSoldier.bBusy = true;
	//reload visuals here
	FTimerHandle TimerHandle_Reload;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, FName("SetAmmo"), FiringSoldier, FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->MaxAmmo);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reload, Delegate, FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->ReloadTime, false);
}


void UGunHandler::SetAmmo(FSoldierPropertyStruct& FiringSoldier, int32 Ammo)
{
	FiringSoldier.bBusy = false;
	FiringSoldier.SoldierWeapon.CurrentAmmo = Ammo;
	Cast<ARTS_Unit_Base>(GetOwner())->SetSoldierData(FiringSoldier.Index, FiringSoldier);
}
// Called every frame
void UGunHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



TArray<FSoldierPropertyStruct> UGunHandler::SetUpGuns()
{
	ARTS_Unit_Base* Owner = Cast<ARTS_Unit_Base>(GetOwner());
	TArray<FSoldierPropertyStruct> UnitProperties = Owner->GetUnitdata();
	Owner->ISMWeaponSoldier->ClearInstances();
	Owner->ISMWeaponOfficer->ClearInstances();
	Owner->ISMWeaponSoldier->SetStaticMesh(Owner->SoldierWeapon->GetDefaultObject<UWeaponData>()->WeaponAsset);
	Owner->ISMWeaponOfficer->SetStaticMesh(Owner->OfficerWeapon->GetDefaultObject<UWeaponData>()->WeaponAsset);

	//i just kinda loop to set up stuff, I store most of the relevant information, so I don't have to redo the checking
	if (Owner->SoldierWeapon != Owner->OfficerWeapon)
	{
		for (auto& i : UnitProperties)
		{


			if (i.SoldierType != ESoldierType::Officer)
			{
				FTransform ParentTransform;
				Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
				i.SoldierWeapon.Index = Owner->ISMWeaponSoldier->AddInstance(ParentTransform);
				i.SoldierWeapon.WeaponISM = Owner->ISMWeaponSoldier;
			}
			else
			{
				FTransform ParentTransform;
				Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
				i.SoldierWeapon.Index = Owner->ISMWeaponOfficer->AddInstance(ParentTransform);
				i.SoldierWeapon.WeaponISM = Owner->ISMWeaponOfficer;
				FSoldierPropertyStruct Test = i;
			}


		}
	}
	else
	{
		for (auto& i : UnitProperties)
		{


		
				FTransform ParentTransform;
				Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
				i.SoldierWeapon.Index = Owner->ISMWeaponSoldier->AddInstance(ParentTransform);
				i.SoldierWeapon.WeaponISM = Owner->ISMWeaponSoldier;
	
			
		}

	}


	return UnitProperties;
}

void UGunHandler::UpdateGuns()
{

	ARTS_Unit_Base* Owner = Cast<ARTS_Unit_Base>(GetOwner());
	TArray<FSoldierPropertyStruct> UnitProperties = Owner->GetUnitdata();
	if (Owner->SoldierWeapon != Owner->OfficerWeapon)
	{
		for (auto& i : UnitProperties)
		{
			FTransform ParentTransform;
			Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
			ParentTransform *= FTransform(i.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->DefaultOffset);
			if (i.SoldierType != ESoldierType::Officer)
			{
				Owner->ISMWeaponSoldier->UpdateInstanceTransform(i.SoldierWeapon.Index,ParentTransform);
			}
			else
			{
				Owner->ISMWeaponOfficer->UpdateInstanceTransform(i.SoldierWeapon.Index, ParentTransform);
			}
		}
		Owner->ISMWeaponOfficer->MarkRenderStateDirty();
		Owner->ISMWeaponSoldier->MarkRenderStateDirty();
	}
	else
	{
		for (auto& i : UnitProperties)
		{
			FTransform ParentTransform;
			Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
			ParentTransform *= FTransform(i.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->DefaultOffset);
			Owner->ISMWeaponSoldier->UpdateInstanceTransform(i.SoldierWeapon.Index, ParentTransform);
		}
		Owner->ISMWeaponSoldier->MarkRenderStateDirty();
	}

}

void UGunHandler::SetUpComponent()
{

}

