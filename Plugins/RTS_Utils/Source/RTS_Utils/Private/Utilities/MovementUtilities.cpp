// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/MovementUtilities.h"
#include "NavigationSystem.h"
#include "RTS_Unit_Base.h"
#include "NavigationPath.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/Object.h"
#include "NavMesh/NavMeshPath.h"
#include "Components//InstancedStaticMeshComponent.h"
#include "Components//CapsuleComponent.h"

#include "Navigation/PathFollowingComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

#include "Utilities/CommonUnitDataTypes.h"


MovementUtilities::MovementUtilities()
{

}

MovementUtilities::~MovementUtilities()
{
}

TArray<FTransform> MovementUtilities::DistributeUnitsAtArea(UNavigationSystemV1* NavSys, FVector const& UnitPosition, TArray<FSoldierPropertyStruct>& SoldierProperties)
{
	TArray<FTransform> Transforms;
	Transforms.Reserve(5);
	if (NavSys == nullptr)
	{
		Transforms.Init(FTransform::Identity, 5);
		return Transforms;
	}

	for (int i = 0; i < 5; ++i)
	{



		FNavLocation NavPos;
		FRotator Rot = FRotator::ZeroRotator;
		FVector Scale(1.0f);
		FVector Pos;
		if (NavSys->GetRandomPointInNavigableRadius(UnitPosition + SoldierProperties[i].FormationOffSet, 50.f, NavPos))
		{


			Pos = NavPos.Location - UnitPosition;

		}
		else
		{
			Pos = FVector(0, 0, 0);
		}
		Transforms.Emplace(Rot, Pos, Scale);
	}
	return Transforms;
}

TObjectPtr<UNavigationPath> MovementUtilities::GeneratePathPoints(UNavigationSystemV1& NavSys, UObject* WorldContext, const FVector& PathStart, const FVector& PathEnd, const float OffSetDistance, AActor* PathFindingContext, TSubclassOf <UNavigationQueryFilter> FilterClass)
{
	UNavigationPath* NavPath = NavSys.FindPathToLocationSynchronously(WorldContext, PathStart, PathEnd, PathFindingContext, FilterClass);

	if (!NavPath->IsValid())
	{
		return NavPath;
	}
	const FNavPathSharedPtr SharedPathPtr = NavPath->GetPath();

	SharedPathPtr.Get()->CastPath<FNavMeshPath>()->OffsetFromCorners(150.f);
	UNavigationPath* Offsetpath = NewObject<UNavigationPath>();
	Offsetpath->SetPath(SharedPathPtr);





	return Offsetpath;
}

TArray<FTransform> MovementUtilities::CalculateMovementTransform(TObjectPtr<ARTS_Unit_Base> Unit, float DeltaTime)
{
	TObjectPtr<UInstancedStaticMeshComponent> ISM = Unit->GetIsm();
	TArray<FSoldierPropertyStruct> UnitInformation = Unit->GetUnitdata();

	TArray<FTransform> MovedUnitTransform;
	FVector SoldierAvoidance = FVector::ZeroVector;
	FVector TargetVector = FVector::ZeroVector;
	FVector ObstacleAvoidance = FVector::ZeroVector;
	FVector FormationVector = FVector::ZeroVector;

	FTransform InstanceTransform;
	FTransform InstanceTransformWorld;
	FNavPathSharedPtr NavPath = Unit->GetPathFollowingComp().GetPath();

	TArray<FNavPathPoint> NavPathPoins = NavPath->GetPathPoints();

	FVector UnitVeloc = Unit->GetVelocity() * DeltaTime * 2.f;

	FCollisionQueryParams TraceParams;

	TraceParams.AddIgnoredActor(Unit);

	for (auto& i : UnitInformation)
	{
		SoldierAvoidance = FVector::ZeroVector;
		TargetVector = FVector::ZeroVector;
		ObstacleAvoidance = FVector::ZeroVector;
		FormationVector = FVector::ZeroVector;
		FTransform OtherSoliderTransform;
		FVector OtherSoldierPosition;


		ISM->GetInstanceTransform(i.Index, InstanceTransform);
		//relativ transform
		FVector Instancelocation = InstanceTransform.GetLocation();
		FVector Distance;


		//Calculate SoliderTransform 
		for (auto OtherSoldier : UnitInformation)
		{
			if (OtherSoldier.Index == i.Index)
			{
				continue;
			}
			ISM->GetInstanceTransform(OtherSoldier.Index, OtherSoliderTransform);
			OtherSoldierPosition = OtherSoliderTransform.GetLocation();

			Distance = (Instancelocation - OtherSoldierPosition);
			if (Distance.Size() < 101)
			{

				float Factor = UKismetMathLibrary::Clamp(80 - Distance.Size(), 1.f, 100.f);
				FVector LocalAvoidance = (Instancelocation - OtherSoldierPosition);
				LocalAvoidance.Normalize(1);
				LocalAvoidance *= Factor;

				//if there is an avoidancemodifcation, add it
				if (OtherSoldier.PathMods.AvoidanceModification != FVector::ZeroVector)
				{
					LocalAvoidance += OtherSoldier.PathMods.AvoidanceModification;
				}


				LocalAvoidance -= FVector(0, 0, LocalAvoidance.Z);
				SoldierAvoidance += LocalAvoidance;
			}
		}

		//calculate the Formation Vector which aligns the solider with his Formation Offset if he crosses the threshold
		Distance = (Instancelocation - i.FormationOffSet);
		if (Distance.Size() > 100)
		{
			float Factor = UKismetMathLibrary::Clamp(Distance.Size(), 0.f, 25.f);
			FormationVector = (i.FormationOffSet - Instancelocation);
			FormationVector.Normalize(1);
			FormationVector -= FVector(0, 0, FormationVector.Z);
			FormationVector *= Factor;
		}
		//Calculate Target Vector which makes them move towards the current target


		FVector TargetLocation = NavPathPoins[Unit->GetPathFollowingComp().GetNextPathIndex()].Location;

		ISM->GetInstanceTransform(i.Index, InstanceTransformWorld, true);
		//relativ transform
		FVector InstancelocationWorld = InstanceTransformWorld.GetLocation();
		Distance = (TargetLocation - InstancelocationWorld);
		if (Distance.Size() > 150)
		{
			float Factor = UKismetMathLibrary::Clamp(Distance.Size(), 0.f, 50.f);
			TargetVector = TargetLocation - InstancelocationWorld;
			TargetVector.Normalize(1);
			TargetVector *= Factor;
		}

		//obstalce avoidance,line trace forward if hit do smth
		//for placing, I would say after moving it triggers a reshuffle phase where formation offsets can be redefined to fit the new pos ition
			   //Obstalce Avoidance Logic
		FVector FinalVector = (TargetVector + SoldierAvoidance + FormationVector);
		FHitResult Trace;

		FVector United = (FinalVector * DeltaTime * 2.f) + UnitVeloc;


		if (United.Size() > 1)
		{
			United = United.GetSafeNormal();
			Unit->GetWorld()->LineTraceSingleByChannel(Trace, InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()), InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()) + United * 100, ECollisionChannel::ECC_Visibility, TraceParams);
			DrawDebugLine(Unit->GetWorld(), InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()), InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()) + United * 100, FColor::Red, false, 0.5f);
			if (Trace.bBlockingHit)
			{

				DrawDebugLine(Unit->GetWorld(), InstancelocationWorld, InstancelocationWorld + United * 100, FColor::Red, false, 10.f);
				FVector Slide = FVector::VectorPlaneProject(United, Trace.Normal);

				ObstacleAvoidance = Slide.GetSafeNormal() * 150.F;
				ObstacleAvoidance += (InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()) - Trace.ImpactPoint).GetSafeNormal(1) * 200;
				DrawDebugLine(Unit->GetWorld(), InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()), InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()) + ObstacleAvoidance, FColor::Green, false, 4.f);
			}

		}


		FinalVector += ObstacleAvoidance;
		FinalVector *= DeltaTime * 2.f;



		//clean the avoidance modifcation so they don't stack or stay forever
		i.PathMods.AvoidanceModification = FVector::ZeroVector;

		Unit->GetWorld()->LineTraceSingleByChannel(Trace, InstancelocationWorld + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()), InstancelocationWorld + FVector::DownVector * Unit->CapsuleComp->GetScaledCapsuleHalfHeight() * 1, ECollisionChannel::ECC_Visibility, TraceParams);
		DrawDebugLine(Unit->GetWorld(), InstancelocationWorld + United + FVector(0, 0, Unit->CapsuleComp->GetScaledCapsuleHalfHeight()), InstancelocationWorld + United + FVector::DownVector * Unit->CapsuleComp->GetScaledCapsuleHalfHeight() * 1, FColor::Magenta, false, 7.f);

		if (Trace.Distance > Unit->CapsuleComp->GetScaledCapsuleHalfHeight() and Trace.Distance < Unit->CapsuleComp->GetScaledCapsuleHalfHeight() * 2)// stuck inside 
		{


			ObstacleAvoidance = Trace.ImpactPoint - InstancelocationWorld;
			FinalVector += ObstacleAvoidance * DeltaTime * 10.f;
			DrawDebugLine(Unit->GetWorld(), InstancelocationWorld, InstancelocationWorld + ObstacleAvoidance, FColor::Black, false, 2.f);
		}
		else if (Trace.bBlockingHit and Trace.Distance < Unit->CapsuleComp->GetScaledCapsuleHalfHeight())//floating
		{

			ObstacleAvoidance = Trace.ImpactPoint - InstancelocationWorld;
			FinalVector += ObstacleAvoidance * DeltaTime * 10.f;
			DrawDebugLine(Unit->GetWorld(), InstancelocationWorld, InstancelocationWorld + ObstacleAvoidance, FColor::Purple, false, 2.f);
		}
		else if (!Trace.bBlockingHit or Trace.Distance > Unit->CapsuleComp->GetScaledCapsuleHalfHeight() * 2) // if moving towards a cliff
		{

			
			
			ObstacleAvoidance = (ISM->GetComponentLocation()) - InstancelocationWorld;

			FinalVector = ObstacleAvoidance * DeltaTime * 10.f;
			i.PathMods.AvoidanceModification = ObstacleAvoidance;
			Unit->SetSoldierData(i.Index, i);
			DrawDebugLine(Unit->GetWorld(), InstancelocationWorld, InstancelocationWorld + ObstacleAvoidance, FColor::Emerald, false, 2.f);
			DrawDebugLine(Unit->GetWorld(), InstancelocationWorld, ISM->GetComponentLocation()+ FinalVector + Instancelocation, FColor::Black, false, 3.f);
		}

		
		FinalVector += Instancelocation;
		//	 UE_LOG(LogTemp, Warning, TEXT("Velocity %s"), *Unit->GetVelocity().ToString())


		FRotator DesiredRotation = UKismetMathLibrary::MakeRotFromXY(FinalVector + UnitVeloc, FinalVector + UnitVeloc);
		DesiredRotation.Pitch = 0;
		DesiredRotation.Roll = 0;
		DesiredRotation += FRotator(0, -90, 0);

		FRotator NewRoation = FMath::RInterpTo(FRotator(InstanceTransform.GetRotation()), DesiredRotation, DeltaTime, 5.f);



		MovedUnitTransform.Emplace(NewRoation, FinalVector, FVector::OneVector);
	}

	return MovedUnitTransform;
}

FTransform MovementUtilities::ChangeUnitRotation(ARTS_Unit_Base* Unit,FRotator NewRotation, int32 Index)
{

	FTransform SoldierPosition;

	NewRotation.Roll = 0;
	NewRotation.Pitch = 0;
	NewRotation += FRotator(0,-90, 0);
	Unit->GetIsm()->GetInstanceTransform(Index, SoldierPosition);
	FTransform FinalPositon = FTransform(NewRotation, SoldierPosition.GetLocation());
	return FinalPositon;
}
