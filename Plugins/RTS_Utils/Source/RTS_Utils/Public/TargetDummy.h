

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "Components/StaticMeshComponent.h"

#include "TargetDummy.generated.h"

UCLASS()
class RTS_UTILS_API ATargetDummy : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayTags, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visuals, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

};
