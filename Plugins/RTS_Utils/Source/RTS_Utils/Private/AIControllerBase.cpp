// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "Perception/AIPerceptionComponent.h"

void AAIControllerBase::BeginPlay()
{
}


AAIControllerBase::AAIControllerBase()
{

	AiPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Ai Perception Component"));



}