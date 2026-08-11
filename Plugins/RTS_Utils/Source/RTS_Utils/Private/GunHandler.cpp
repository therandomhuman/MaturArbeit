// Fill out your copyright notice in the Description page of Project Settings.


#include "GunHandler.h"
#include "RTS_Unit_Base.h"
#include "Utilities/CommonUnitDataTypes.h"
#include  "Components//InstancedStaticMeshComponent.h"

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

void UGunHandler::FireGun(FSoldierPropertyStruct& FiringSoldier)
{
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
	UE_LOG(LogTemp,Warning,TEXT("COVER ME I'M RELOADING"))
	FiringSoldier.bBusy = true;
	//reload visuals here
	FTimerHandle TimerHandle_Reload;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, FName("SetAmmo"), FiringSoldier, FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->MaxAmmo);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reload, Delegate, FiringSoldier.SoldierWeapon.GunDataAsset->GetDefaultObject<UWeaponData>()->ReloadTime, false);
}



// Called every frame
void UGunHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunHandler::SetAmmo(FSoldierPropertyStruct& FiringSoldier, int32 Ammo)
{
	FiringSoldier.bBusy = false;
	FiringSoldier.SoldierWeapon.CurrentAmmo = Ammo;
	Cast<ARTS_Unit_Base>(GetOwner())->SetSoldierData(FiringSoldier.Index, FiringSoldier);
}

TArray<FSoldierPropertyStruct> UGunHandler::SetUpGuns()
{
	ARTS_Unit_Base* Owner = Cast<ARTS_Unit_Base>(GetOwner());
	TArray<FSoldierPropertyStruct> UnitProperties = Owner->GetUnitdata();
	Owner->ISMWeaponSoldier->ClearInstances();
	Owner->ISMWeaponOfficer->ClearInstances();
	Owner->ISMWeaponSoldier->SetStaticMesh(Owner->SoldierWeapon->GetDefaultObject<UWeaponData>()->WeaponAsset);
	Owner->ISMWeaponOfficer->SetStaticMesh(Owner->OfficerWeapon->GetDefaultObject<UWeaponData>()->WeaponAsset);


	if (Owner->SoldierWeapon != Owner->OfficerWeapon)
	{
		for (auto& i : UnitProperties)
		{


			if (i.SoldierType != ESoldierType::Officer)
			{
				FTransform ParentTransform;
				Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
				i.SoldierWeapon.Index = Owner->ISMWeaponSoldier->AddInstance(ParentTransform);

			}
			else
			{
				FTransform ParentTransform;
				Owner->GetIsm()->GetInstanceTransform(i.Index, ParentTransform);
				i.SoldierWeapon.Index = Owner->ISMWeaponOfficer->AddInstance(ParentTransform);
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

